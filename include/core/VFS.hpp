//
// Created by ethan on 27/4/26.
//

#ifndef DEXIUM_VFS_HPP
#define DEXIUM_VFS_HPP

#include <filesystem>
#include <mutex>
#include <string_view>

class VFS {
public:
    VFS();

    std::filesystem::path getWorkingPath();
    bool updateWorkingPath(std::filesystem::path path);
    bool updateWorkingPath(std::string_view path);

    // Returns an immutable path to the execPath(Including the execName).
    const std::filesystem::path& getExecPath();

    // Take a relative path and attempt to resolve it into an absolute path. Fails if the relative path cannot be found.
    std::filesystem::path resolve(const std::filesystem::path& relPath) const;

    static bool exists(const std::filesystem::path& path);

private:

    std::filesystem::path m_execPath;
    std::filesystem::path m_workingPath;

    std::mutex m_workingPathMutex;

};

/*
class VFS {
public:
    VFS();

    std::filesystem::path resolveRel(std::string_view relativePath);

    std::filesystem::path updateRtimenPath(std::string_view absPath);
    std::filesystem::path updateRtimenPath(std::filesystem::path absPath);

    bool exists(std::string_view path);

    const std::filesystem::path& getRtimePath();

private:
    std::filesystem::path execPath;

};
*/

#endif //DEXIUM_VFS_HPP