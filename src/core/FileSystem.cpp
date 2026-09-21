//
// Created by ethan on 6/9/26.
//


#include <core/FileSystem.hpp>
#include <util/Logger.hpp>

#if defined(__linux__)
#include <system_error>
#endif

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Dexium::Core {
    Filesystem::Filesystem() {
        // Begin Linux Specifc code to read exec location
#if defined(__linux__)
        std::error_code ec;
        m_execPath = std::filesystem::read_symlink("/proc/self/exe", ec);

        if (ec) {
            TraceLog(LogLevel::ERROR, "Failed to read executable path!\nSome functionality may be limited!");
        }
#endif

        //Begin Windows Specific code to read exec lcoation
#if defined(_WIN32)
        wchar_t buffer[MAX_PATH];
        GetModuleFilenameW(NULL, buffer, MAX_PATH);

        m_execDir = std::filesystem::path(buffer);
#endif

        // Ensure execPath is absolute
        m_execPath = std::filesystem::absolute(m_execPath);


        // Set the default working directory tot he same as exec - execName
        m_workingDir = m_execPath.parent_path();
    }

    const std::filesystem::path& Filesystem::getExecDir() const {
        return m_execPath;
    }

    const std::filesystem::path& Filesystem::getWorkingDir() const {
        return m_workingDir;
    }

    void Filesystem::setWorkingDir(const std::filesystem::path& path) {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            m_workingDir = path;
        } else {
            TraceLog(LogLevel::WARN, "Request to change working dir to {} denied, not a suitable directory", path.c_str());
        }
    }

    std::filesystem::path Filesystem::resolve(const std::filesystem::path& path) {
        std::filesystem::path res;

        // Check if the path is absolute or relative
        if (path.is_absolute()) {
            res = path;
        } else {
            // Join rel path onto workingPath
            res = m_workingDir / path;
        }

        // Strim relative apth elements like '../'
        res = std::filesystem::weakly_canonical(res);

        // Check that a potential resolved path exists
        if (!std::filesystem::exists(res)) {
            TraceLog(LogLevel::WARN, "Failed to resolve {}. Attempted result --> {}", path.c_str(), res.c_str());
            return {};
        }

        // Check that a potential resolved path is a directory
        if (!std::filesystem::is_directory(res)) {
            TraceLog(LogLevel::WARN, "Failed to resolve {}. Attempted path {} is not a directory", path.c_str(), res.c_str());
            return {};
        }

        // Return reoslved path
        return res;
    }
}
