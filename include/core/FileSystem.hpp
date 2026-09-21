//
// Created by ethan on 5/9/26.
//

#ifndef DEXIUM_FILESYSTEM_HPP
#define DEXIUM_FILESYSTEM_HPP

#include <filesystem>

namespace Dexium::Core {

    class Filesystem {
    public:
        Filesystem();

        std::filesystem::path resolve(const std::filesystem::path& path);

        const std::filesystem::path& getExecDir() const;

        const std::filesystem::path& getWorkingDir() const;

        void setWorkingDir(const std::filesystem::path& path);

    private:
        std::filesystem::path m_execPath;
        std::filesystem::path m_workingDir;
    };

}

#endif //DEXIUM_FILESYSTEM_HPP
