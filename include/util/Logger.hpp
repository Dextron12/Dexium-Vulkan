//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_LOGGER_HPP
#define DEXIUM_VULKAN2_LOGGER_HPP

#include <chrono>
#include <optional>

#include <string>
#include <unordered_map>

#include <mutex>
#include <thread>

#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#if __cplusplus >= 202002L
#include <source_location>
#endif

#include <fmt/base.h>
#include <fmt/color.h>

#include "core/ServiceLocator.hpp"

enum class LogLevel {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
};

struct Formatter {

    template<typename... Args>
    std::string format(const LogLevel& lvl, fmt::format_string<Args...> message, Args&&... args) const {
        return fmt::format(selectStyle(lvl), message, std::forward<Args>(args)...);
    }

    const std::unordered_map<LogLevel, fmt::text_style> styles{
        {LogLevel::TRACE, fmt::fg(fmt::color::gray)},
        {LogLevel::DEBUG, fg(fmt::color::light_blue)},
        {LogLevel::INFO, fg(fmt::color::light_green)},
        {LogLevel::WARN, fg(fmt::color::yellow) | fmt::emphasis::italic},
        {LogLevel::ERROR, fg(fmt::color::red) | fmt::emphasis::bold},
        {LogLevel::FATAL, fg(fmt::color::medium_purple) | fmt::emphasis::bold}
    };

    // Time formatting
    bool showTime = true;
    std::string timeFormat = "[{:%H:%M}]";

    //LogLevel formatting
    bool showLogLevel = true;
    std::optional<fmt::text_style> LogLevelStyle = std::nullopt;
    const std::unordered_map<LogLevel, std::string> LogLevelStrings{
        {LogLevel::TRACE, "Trace"},
        {LogLevel::DEBUG, "Debug"},
        {LogLevel::INFO, "Info"},
        {LogLevel::WARN, "Warn"},
        {LogLevel::ERROR, "Error"},
        {LogLevel::FATAL, "Fatal"}
    };

    bool showLocation = false;

    fmt::text_style selectStyle(const LogLevel& lvl) const{
        fmt::text_style result;

        try {
            result = styles.at(lvl);
        } catch (const std::out_of_range& e) {
            // Output error msg to engine logger
            result = styles.at(LogLevel::ERROR);
        }

        return result;
    }
};

struct LogRecord {
    std::chrono::system_clock::time_point timestamp;
    LogLevel level;
    std::string message;
    std::source_location location;

    LogRecord(LogLevel level, std::string_view message, std::source_location location = std::source_location::current()) :
    level(level), message(message), location(location), timestamp(std::chrono::system_clock::now()) {}

};

class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    virtual void write(const LogRecord& record, const Formatter& format) = 0;
};


class StderrSink : public Sink {
    public:
    void write(const LogRecord& record, const Formatter& format) override {
        std::lock_guard<std::mutex> lock(writeMutex);
        std::string prefix;


        // Time module
        if (format.showTime) {
            // Convert the UTC timestamp to localtime
            auto local = std::chrono::current_zone()->to_local(record.timestamp);
            prefix = fmt::format(fmt::runtime(format.timeFormat), local);
        }

        //LogLevel module
        if (format.showLogLevel) {
            prefix += "[" + format.LogLevelStrings.at(record.level) + "]";
        }

        // Location module
        //prefix += " from: {}::{}(ln: {})"
        if (format.showLocation) {
            prefix += fmt::format(" from: {}, {}(ln: {})", record.location.file_name(), record.location.function_name(), record.location.line());
        }

        // Get Text style
        auto style = format.selectStyle(record.level);
        fmt::print(stderr, style, "{}: {}\n", prefix, record.message);
    }

private:
    std::mutex writeMutex;
};


class Logger {
public:
    Logger(std::optional<Formatter> formatter = std::nullopt) {
        if (formatter.has_value()) {
            this->formatter = std::make_unique<Formatter>(formatter.value());
        }
    }

    template<typename... Args>
    void write(LogLevel lvl, std::source_location loc, fmt::format_string<Args...> message, Args&&... args) {
        // Use preset formatter or use a default format if one not set
        auto fmt = formatter == nullptr ? Formatter{} : *formatter;

        // Build record
        LogRecord record{lvl, fmt.format(lvl, message, std::forward<Args>(args)...), loc};

        // Write record to sink & discard
        for (auto& sink : sinks) {
            sink->write(record, fmt);
        }
    }

    template<typename... Args>
    void write(LogLevel lvl, const Formatter& fmt, std::source_location loc, fmt::format_string<Args...> message, Args&&... args) {
        // Build record
        LogRecord record{lvl, fmt.format(lvl, message, std::forward<Args>(args)...), loc};

        // Write to sinks
        for (auto& sink : sinks) {
            sink->write(record, fmt);
        }
    }

    void setFormatter(Formatter format) {
        formatter = std::make_unique<Formatter>(std::move(format));
    }

    [[nodiscard]] Formatter& getFormatter() const { return *formatter;}

    void connectSink(std::unique_ptr<Sink> sink) {
        sinks.push_back(std::move(sink));
    }

protected:
    std::unique_ptr<Formatter> formatter;
    std::vector<std::unique_ptr<Sink>> sinks;
};

template<typename... Args>
void TraceLogImpl(LogLevel lvl, fmt::format_string<Args...> message, std::source_location loc, Args&&... args) {
    //Access the locator<Logger>
    auto& logger = Dexium::Core::serviceLocator.get<Logger>();

    logger.write(lvl, loc, message, std::forward<Args>(args)...);
}

template<typename... Args>
void TraceLogImpl(LogLevel lvl, const Formatter& format, fmt::format_string<Args...> message, std::source_location loc, Args&&... args) {
    //Find Logger service
    auto& logger = Dexium::Core::serviceLocator.get<Logger>();

    logger.write(lvl, format, loc,  message, std::forward<Args>(args)...);
}


#define TraceLogFormat(lvl, format, message, ...) \
    TraceLogImpl(lvl, format, message, std::source_location::current(), ##__VA_ARGS__)

#define TraceLog(lvl, message, ...) \
    TraceLogImpl(lvl, message, std::source_location::current(), ##__VA_ARGS__)

#endif //DEXIUM_VULKAN2_LOGGER_HPP