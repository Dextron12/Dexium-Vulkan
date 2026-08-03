//
// Created by ethan on 29/7/26.
//

#ifndef DEXIUM_VERSIONCONTROL_HPP
#define DEXIUM_VERSIONCONTROL_HPP

#include <cstdint>
#include <vulkan/vulkan_core.h>

namespace Dexium {
    struct VersionControl {
        uint8_t variant = 0;
        uint8_t major = 0;
        uint8_t minor = 0;
        uint8_t patch = 0;

        VersionControl(uint8_t variant, uint8_t major, uint8_t minor, uint8_t patch) : variant(variant), major(major), minor(minor), patch(patch) {}
        VersionControl(uint8_t major, uint8_t minor, uint8_t patch) : major(major), minor(minor), patch(patch) {}

        uint32_t makeVersion() const {
            return VK_MAKE_VERSION(major, minor, patch);
        }

        uint32_t makeAPIVersion() const {
            return VK_MAKE_API_VERSION(variant, major, minor, patch);
        }
    };

    const VersionControl ENGINE_VERSION{0,0,0,0};
}

#endif //DEXIUM_VERSIONCONTROL_HPP