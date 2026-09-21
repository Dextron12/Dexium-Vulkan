//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_DEVICE_HPP
#define DEXIUM_VULKAN2_DEVICE_HPP

#include <Instance.hpp>

namespace Dexium::Vulkan {

    class Device {
    public:
        Device(Instance* instance);

    private:
        VkPhysicalDeviceProperties m_Properties;
        VkPhysicalDeviceFeatures m_features;
    };
}

#endif //DEXIUM_VULKAN2_DEVICE_HPP