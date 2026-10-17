//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_DEVICE_HPP
#define DEXIUM_VULKAN2_DEVICE_HPP

#include <RenderBackends/Vulkan/include/Instance.hpp>

namespace Dexium::Vulkan {

    class Device {
    public:
        // Device properties
        VkPhysicalDeviceProperties properties; // Auto-populated on device creation

        // Device extensionns
        uint32_t extensionCount;
        std::vector<VkExtensionProperties> availableExtensions;

        // Queue family properties
        uint32_t queueFamilyCount;
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        bool supportsGraphicsQueue() const;


        struct DeviceCompatSummary {
            uint32_t vkMinVersion;
            uint32_t vkVersionSupport;


        };

        // Create a Dx::Device(A implementation that manages the creation and destruction of a virtual/logical device & provides some useful helper fn's)
        Device(VkPhysicalDevice pDeviceHandle);

        VkPhysicalDevice getPDevice() const; // returns the handle to the physical device
        VkDevice get() const; // Returns the handle to logical device

        std::vector<const char*> checkExtensionSupport(const std::vector<const char*>& requiredExtensions);

    private:
        VkPhysicalDeviceProperties m_Properties;
        VkPhysicalDeviceFeatures m_features;

        VkPhysicalDevice m_pDevice; // Physical device handle
        VkDevice m_Device; // logical device handle
    };
}

#endif //DEXIUM_VULKAN2_DEVICE_HPP