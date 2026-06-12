//
// Created by ethan on 12/6/26.
//

#ifndef DEXIUM_DEVICE_HPP
#define DEXIUM_DEVICE_HPP


#include <vulkan/vulkan.h>

#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>

namespace Dexium::Vulkan {

    // Automatically selects most suitable physical device and creates the logical device.
    class Device {
    public:
        Device() = default;

        struct DeviceFeatureSet {
            VkPhysicalDeviceFeatures2 vulkanFeatures2{};

            VkPhysicalDeviceVulkan11Features vulkan11Features{};
            VkPhysicalDeviceVulkan12Features vulkan12Features{};
            VkPhysicalDeviceVulkan13Features vulkan13Features{};
            VkPhysicalDeviceVulkan14Features vulkan14Features{};
        };

        struct DeviceFeatureRequest {
            DeviceFeatureSet requiredFeatures; // The required features needed for your application
            std::optional<DeviceFeatureSet> optFeatures; // Features that (if enabled) would extend your application but are not neccessary
        };

        class FeatureChainBuilder {
        public:
            FeatureChainBuilder& add(VkPhysicalDeviceFeatures2& s);
            FeatureChainBuilder& add(VkPhysicalDeviceVulkan11Features& s);
            FeatureChainBuilder& add(VkPhysicalDeviceVulkan12Features& s);
            FeatureChainBuilder& add(VkPhysicalDeviceVulkan13Features& s);

            // Extension chain overrides:
            //FeatureChainBuilder& add();

            // Constructs the StructureChain. IMPORTANT: All features that are added must outlive the fn/CreateInfo that consumes this StructureChain (or will cause internal dangling ptrs)
            VkPhysicalDeviceFeatures2* build();

        private:
            VkBaseOutStructure* head = nullptr; // The top-level struct of the chain(Think of the chain as a tree)
            VkBaseOutStructure* tail = nullptr;

            // The base implementation for all add fn's
            FeatureChainBuilder& addStructure(VkBaseOutStructure* s);

        };

        std::unique_ptr<VkPhysicalDevice> pDevice = nullptr; // The physical selected device(GPU)
        std::unique_ptr<VkPhysicalDevice> device = nullptr; // The logical device over pDevice

        // Queries all available devices. On most systems there is usually one device found
        static std::vector<VkPhysicalDevice> queryDevices(VkInstance& instance);

        // Iterates over all queried devices & returns a list of devices that meet the minimum engine requirements
        [[nodiscard]] std::vector<VkPhysicalDevice> minDeviceRequirements(VkInstance& instance, DeviceFeatureSet& requiredFeatures);

        // Takes a list of devices that meet minimum device requirements & filters out devices that don't meet the requiredFeature list in DeviceFeatureRequest
        [[nodiscard]] std::vector<VkPhysicalDevice> filterDevices(std::vector<VkPhysicalDevice>& deviceMinRequirements, DeviceFeatureRequest& featureRequest);

        // OPTIONALLY score the devices based off of minumum requirements & optionally supported features/extensions
        [[nodiscard]] std::unordered_map<VkPhysicalDevice, float> scoreDevicesBySupport(DeviceFeatureSet& optDeviceFeatures);

        // Find a compatible device(Supports minium engine features & requested features -> then ranks devices by optional features -> chosoes the highest) & construct a logical device
        void constructDevice(DeviceFeatureRequest& featureRequest);
    };
}

#endif //DEXIUM_DEVICE_HPP