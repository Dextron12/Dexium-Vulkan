//
// Created by ethan on 27/4/26.
//

#ifndef DEXIUM_VFS_HPP
#define DEXIUM_VFS_HPP

#include <filesystem>
#include <string_view>

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

#endif //DEXIUM_VFS_HPP