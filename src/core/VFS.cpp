//
// Created by ethan on 27/4/26.
//

#include <iostream>
#include <core/VFS.hpp>
#include <core/Error.hpp>

#ifdef __linux__
#include  <unistd.h>
#include <linux/limits.h>
#endif


VFS::VFS() {
    using LogLevel = Dexium::Core::LogLevel;
#ifdef __linux__
    char buffer[PATH_MAX];

    ssize_t len = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (len == -1) {
        TraceLog(LogLevel::FATAL, "Failed to read executable symbolic link!");
    }

    buffer[len] = '\0'; // Add string null-termination
    m_execPath = std::filesystem::path(buffer);
#endif

    // Need to add a windows specific execPath finder

    // Set working path to be execPath stripped of its execName
    m_workingPath = m_execPath.parent_path();

}

std::filesystem::path VFS::getWorkingPath() const {
    if (m_workingPath.empty()) {
        TraceLog(Dexium::Core::LogLevel::WARN, "Working path is not provided! Please manually update the working path!");
        return std::filesystem::path();
    }

    return m_workingPath;
}

bool VFS::updateWorkingPath(std::filesystem::path path) {
    if (path.empty()) {
        return false;
    }

    if (std::filesystem::exists(path)) {
        m_workingPath = path;
        return true;
    }

    // Otherwise, try resolve a potentially relative path:
    auto resPath = this->resolve(path);
    if (!resPath.empty()) {
        m_workingPath = resPath;
        return true;
    }

    // Failed to update the working path:
    return false;
}

bool VFS::updateWorkingPath(std::string_view path) {
    return updateWorkingPath(std::filesystem::path(path));
}

const std::filesystem::path& VFS::getExecPath() const {
    return m_execPath;
}

bool VFS::exists(const std::filesystem::path& path) {
    if (path.empty()) return false;

    if (std::filesystem::exists(path)) return true;

    return false;
}

std::filesystem::path VFS::resolve(const std::filesystem::path& relPath) const {
    if (relPath.empty()) return std::filesystem::path();

    std::filesystem::path res;

    // Check if the path is absolute:
    if (relPath.is_absolute()) {
        //Ensure it exists:
        if (std::filesystem::exists(relPath)) res = relPath;
        else {
            if (relPath.has_root_directory() && !relPath.has_root_name()){
                res = (m_workingPath / relPath.relative_path()).lexically_normal();
            }

        }
    }

    // Other wise treat as a relative path from the working dir
    if (relPath.is_relative()) {
        res = (m_workingPath / relPath).lexically_normal();
    }

    if (std::filesystem::exists(res)) {
        return res;
    } else {
        // Try resolve suing canonical apthr esolution
        try {
            res = std::filesystem::canonical(res);
            return res;
        } catch (const std::exception& ex) {
            //TraceLog(Dexium::Core::LogLevel::WARN, "Failed to canonically resolve the path: '{}'\nException(): {}", res.c_str(), ex.what());
        }
    }

    // failed to resolve path.
    return std::filesystem::path();
}