//
// Created by ethan on 9/9/26.
//

#ifndef DEXIUM_VERSIONCONTROL_HPP
#define DEXIUM_VERSIONCONTROL_HPP

#include <cstdint>

namespace Dexium::Core {

    class VersionControl {
    public:
        VersionControl(uint32_t major, uint32_t minor, uint32_t patch)
            : variant(0), major(major), minor(minor), patch(patch) {}

        VersionControl(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch)
            : variant(variant), major(major), minor(minor), patch(patch) {}

        uint32_t makeVersion() const {
            return ((((uint32_t)(major)) << 22U) | (((uint32_t)(minor)) << 12U) | ((uint32_t)(patch)));
        }

        uint32_t makeApi() const {
            return ( ( (static_cast<uint32_t>(variant)) << 29U) | ( (static_cast<uint32_t>(major)) << 22U) | ( (static_cast<uint32_t>(minor)) << 12U) | ( (static_cast<uint32_t>(patch))));
        }

    protected:
        uint32_t variant;
        uint32_t major;
        uint32_t minor;
        uint32_t patch;
    };
}

#endif //DEXIUM_VERSIONCONTROL_HPP
