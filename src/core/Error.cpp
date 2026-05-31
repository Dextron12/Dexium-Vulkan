//
// Created by ethan on 24/5/26.
//

#include <core/Error.hpp>

#include <fmt/format.h>
#include <fmt/chrono.h>

namespace Dexium::Core {
    Logger::Logger() {
        // Create an stderr(default) sink
        addSink(std::make_unique<StderrSink>());
    }

    int Logger::addSink(std::unique_ptr<Sink> sink) {
        // Move sink
        m_sinks.emplace_back(std::move(sink));

        // return a ID
        return m_sinks.size();
    }

    Sink* Logger::fetchSink(int sinkID) {
        if (m_sinks.empty()) return nullptr;

        if (m_sinks[sinkID] == nullptr) {
            return nullptr;
        }

        return m_sinks[sinkID].release();

    }

    void Logger::destroySink(int sinkID) {
        if (m_sinks.empty()) return;
        m_sinks.erase(m_sinks.begin() + sinkID);
    }

    void Logger::log(const LogMsg& msg) {
        if (m_sinks.empty()) {
            m_sinks.emplace_back(std::make_unique<StderrSink>());
        }

        Formatter defaultFormatter;
        log(msg, defaultFormatter);
    }

    void Logger::log(const LogMsg& msg, const Formatter& formatter) {
        // Pass output off to ALL registered sinks. Each sink will handle their own formatting
        for (const auto& sink : m_sinks) {
            sink->write(msg, formatter);
        }
    }

    void StderrSink::write(const LogMsg& msg, const Formatter& format) {
        std::string prefix, suffix;
        fmt::color finalColor;

        // Apply timestamp
        if (format.useTimestampPrefixes) {
            prefix += fmt::format("{:%Y-%m-%d] [%H:%M:%S] }", msg.timestamp);
        }

        //Apply LogLevel tag
        if (format.useLogLevelPrefixes) {
            switch (msg.level) {
            case LogLevel::TRACE:
                prefix += "[TRACE]: ";
                finalColor = format.colorSpec.trace;
                break;
            case LogLevel::DEBUG:
                prefix += "[DEBUG]: ";
                finalColor = format.colorSpec.debug;
                break;
            case LogLevel::INFO:
                prefix += "[INFO]: ";
                finalColor = format.colorSpec.info;
                break;
            case LogLevel::WARN:
                prefix += "[WARN]: ";
                finalColor = format.colorSpec.warn;
                break;
            case LogLevel::ERROR:
                prefix += "[ERROR]: ";
                finalColor = format.colorSpec.error;
                break;
            case LogLevel::FATAL:
                prefix += "[FATAL]: ";
                finalColor = format.colorSpec.fatal;
                break;
            }
        }

        // Apply Cursor:
        if (format.useCursorOnOutput) {
            for (const auto& lvlCur : format.showCursorOnLevels) {
                if (msg.level == lvlCur) {
                    prefix += format.cursor; break;
                }
            }
        }

        // Ensure each mesg terminates to a newline
        suffix = "\n";

        // Output to stderr
        fmt::print(stderr, fg(finalColor), "{} {}{}", prefix, msg.msg, suffix);
    }
}
