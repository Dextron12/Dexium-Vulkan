//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_DEVICE_HPP
#define DEXIUM_VULKAN2_DEVICE_HPP

#include <RenderBackends/Vulkan/include/Instance.hpp>
#include <RenderBackends/Vulkan/include/VkExtensionResult.hpp>

namespace Dexium::Vulkan {

    // Request the type of queue family for your use-case. See [Vulkan Documentation: vkGetPhysicalDeviceQueueFamilyProperties](https://docs.vulkan.org/refpages/latest/refpages/source/vkGetPhysicalDeviceQueueFamilyProperties.html)
    struct QueueRequest {
        uint32_t queueCount = 0; // Will create exact number of requested queues from the family, provided it's within the MAX limit set by QueueFamilyInfo::queueCount, otherwise will create MAX queues

        VkQueueFlags requiredCapabilities;

        bool requireGraphics = false;
        bool requireCompute = false;
        bool requirePresent = false;
        bool requireTransfer = false;

        bool requireTransferOnly = false; // Ensures only a transfer queue family is selected(If one available)

        QueueRequest(uint32_t queueCount, VkQueueFlags requiredCapabilities = VK_QUEUE_GRAPHICS_BIT) : queueCount(queueCount), requiredCapabilities(requiredCapabilities) {}
    };


    struct QueueFamilyInfo {
        uint32_t index = ~0; // The QueueFamily index to VkQueueFamilyProperties vec
        uint32_t queueCount = 0; // Number of queues this family supports

        uint32_t createQueues = 0; // The number of queues to create from the family. This value si entirely user-driven and signals to Vk how many queues shopuld be created at logical device creation

        VkQueueFlags capabilities;

        bool supportsGraphics = false;
        bool supportsCompute = false;

        VkBool32 queryPresentationSupport(VkPhysicalDevice pDevice, VkSurfaceKHR renderSurface) const;
    };

    class physicalDevice {
    public:
        // Device properties
        VkPhysicalDeviceProperties properties; // Auto-populated on device creation

        // Device extensionns
        uint32_t extensionCount;
        std::vector<VkExtensionProperties> availableExtensions;

        // Queue family properties
        uint32_t queueFamilyCount;
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;


        struct DeviceCompatSummary {
            uint32_t vkMinVersion;
            uint32_t vkVersionSupport;
        };

        // Create a Dx::Device(A implementation that manages the creation and destruction of a virtual/logical device & provides some useful helper fn's)
        physicalDevice(VkPhysicalDevice pDeviceHandle);

        VkPhysicalDevice get() const; // returns the handle to the physical device

        std::vector<QueueFamilyInfo> queryQueueFamilies();

        // Returns a queue family that supports graphics, compute, transfer & present, with the most available queues. Otherwise, returns the next best one
        QueueFamilyInfo getDefaultQueueFamily(VkSurfaceKHR renderSurface);

        // Request a queueFamily that support specific features. Refer to QueueRequest for what can be requested
        QueueFamilyInfo requestQueueFamily(const QueueRequest& request, VkSurfaceKHR renderSurface);


        DX_EXT_PACK checkExtensionSupport(const std::vector<const char*>& requiredExtensions);

    private:
        VkPhysicalDeviceProperties m_Properties;
        VkPhysicalDeviceFeatures m_features;

        VkPhysicalDevice m_Device; // Physical device handle

        void queryPhysicalDeviceQfProperties();
    };
}

#endif //DEXIUM_VULKAN2_DEVICE_HPP