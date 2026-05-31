//
// Created by ethan on 27/4/26.
//

#include <iostream>
#include <core/VFS.hpp>

#ifdef __linux__
#include  <unistd.h>
#include <linux/limits.h>
#endif



VFS::VFS() {
#ifdef __linux__

    char buffer[PATH_MAX] = {0};
    ssize_t len = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (len == -1) {
        std::cerr << "ReadLink error" << std::endl;
    }

    buffer[len] = '\0'; // Add string null-termiantion
    execPath = std::filesystem::path(buffer);
#endif
}

// Should eventually changer to return a failState (Struct -> Error.hpp)
std::filesystem::path VFS::updateRtimenPath(std::string_view absPath) {
    if (std::filesystem::exists(absPath.data())) {
        execPath = std::filesystem::path(absPath.data());
    } else {
        std::cerr << "Cannot change the runtime path to: " << absPath.data() << " it is invalid" << std::endl;
    }
    return execPath;
}

std::filesystem::path VFS::updateRtimenPath(std::filesystem::path absPath) {
    return updateRtimenPath(std::string_view(absPath.string()));
}

std::filesystem::path VFS::resolveRel(std::string_view relativePath) {
    if (execPath.empty()) {
        throw std::runtime_error("Runtime environment path is invalid. Something is wrong");
    }

    std::filesystem::path path(relativePath.data());

    if (path.is_absolute()) {
        if (std::filesystem::exists(path)) return path;
        else {
            if (path.has_root_directory() && !path.has_root_name()) {
                return (execPath / path.relative_path()).lexically_normal();
            }
        }

        std::cerr << "Identiefied " << path << " as an abs path, but can not validate the path";
        return {};
    }

    // Otherwise treat as relative path from the virtual root
    auto abs = (execPath / path).lexically_normal();
    if (std::filesystem::exists(abs)) return abs;

    return {};
}

bool VFS::exists(std::string_view path) {

    return resolveRel(path).empty();
}

const std::filesystem::path& VFS::getRtimePath() {
    return execPath;
}
