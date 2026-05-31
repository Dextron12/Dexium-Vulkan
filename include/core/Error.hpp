//
// Created by ethan on 24/5/26.
//

#ifndef DEXIUM_ERROR_HPP
#define DEXIUM_ERROR_HPP

#include <fmt/base.h>
#include <fmt/color.h>

#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

#include <queue>
#include <list>

#include <ostream>

namespace Dexium::Core {

    enum class LogLevel {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    struct LogMsg {
        std::chrono::system_clock::time_point timestamp;
        std::thread::id threadID;

        LogLevel level;
        std::string msg;
    };

    class Formatter {
    public:
        Formatter() = default;

        struct ColorSpec {
            fmt::color trace = fmt::color::deep_sky_blue;
            fmt::color debug = fmt::color::green;
            fmt::color info = fmt::color::deep_sky_blue;
            fmt::color warn = fmt::color::yellow;
            fmt::color error = fmt::color::red;
            fmt::color fatal = fmt::color::purple;
        };
        ColorSpec colorSpec;

        std::string cursor = "==>";

        std::list<LogLevel> showCursorOnLevels = {
            LogLevel::FATAL,
            LogLevel::ERROR,
            LogLevel::DEBUG,
            LogLevel::WARN
        };

        bool useLogLevelPrefixes = true;
        bool useTimestampPrefixes = false;
        bool useCursorOnOutput = true;
    };

    // Template for custom sink implementation
    class Sink {
    public:
        Sink() = default;
        virtual ~Sink() = default;

        virtual void write(const LogMsg& msg, const Formatter& format) = 0;
    };


    class StderrSink : public Sink {
    public:
        std::ostream& os = std::cerr;

        void write(const LogMsg& msg, const Formatter& format) override;
    };

    class Logger {
    public:
        static Logger& get() {
            static Logger logger;
            return logger;
        }

        // Will take ownership of the sink (returns an ID that can be used to later fetch the sink)
        int addSink(std::unique_ptr<Sink> sink);

        // Removes the sink from the logger adn returns it.
        Sink* fetchSink(int sinkID);

        // Destroys the owned sink without returning a copy.
        void destroySink(int sinkID);

        void log(const LogMsg& msg);
        void log(const LogMsg& msg, const Formatter& formatter);

    private:

        Logger();

        std::queue<LogMsg> m_logQueue;
        std::vector<std::unique_ptr<Sink>> m_sinks;
    };
}

template<typename... Args>
void TraceLog(Dexium::Core::LogLevel level, fmt::format_string<Args...> fmt_str, Args&&... args) {
    auto& instance = Dexium::Core::Logger::get();

    std::string msg = fmt::format(fmt_str, std::forward<Args>(args)...);
    instance.log({
    std::chrono::system_clock::now(),
    std::this_thread::get_id(),
    level,
    msg});
}

template<typename... Args>
void TraceLog(Dexium::Core::LogLevel level, const Dexium::Core::Formatter& formatter,  fmt::format_string<Args...> fmt_str, Args&&... args) {
    auto& instance = Dexium::Core::Logger::get();

    std::string msg = fmt::format(fmt_str, std::forward<Args>(args)...);

    instance.log({
    std::chrono::system_clock::now(),
    std::this_thread::get_id(),
    level,
    msg},
    formatter);
}

#endif //DEXIUM_ERROR_HPP