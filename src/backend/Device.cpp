//
// Created by ethan on 12/6/26.
//

#include <backend/Device.hpp>

#include <core/Error.hpp>

#include <vulkan/vk_enum_string_helper.h> // Provides string_enum(enum value), to stringify pesky Vk enums!

namespace Dexium::Vulkan {

    // INVARIANT: Assumes that VkPhysicalDeviceFeatures2 is always the head
    Device::FeatureChainBuilder& Device::FeatureChainBuilder::addStructure(VkBaseOutStructure* s) {
        if (!head) {
            head = s; // Make this struct the top-most structure fo the chain
            tail = s;
            s->pNext = nullptr;
        } else {
            // This struct is a child within the chain
            tail->pNext = s; // Sets that previous struct pNext to incoming struct
            tail = s; // Updates the tail to this struct
            s->pNext = nullptr;
        }
        return *this;
    }

    Device::FeatureChainBuilder& Device::FeatureChainBuilder::add(VkPhysicalDeviceFeatures2& s) {
        return addStructure(reinterpret_cast<VkBaseOutStructure*>(&s));
    }

    Device::FeatureChainBuilder& Device::FeatureChainBuilder::add(VkPhysicalDeviceVulkan11Features& s) {
        return addStructure(reinterpret_cast<VkBaseOutStructure*>(&s));
    }

    Device::FeatureChainBuilder& Device::FeatureChainBuilder::add(VkPhysicalDeviceVulkan12Features& s) {
        return addStructure(reinterpret_cast<VkBaseOutStructure*>(&s));
    }

    Device::FeatureChainBuilder& Device::FeatureChainBuilder::add(VkPhysicalDeviceVulkan13Features& s) {
        return addStructure(reinterpret_cast<VkBaseOutStructure*>(&s));
    }

    VkPhysicalDeviceFeatures2* Device::FeatureChainBuilder::build() {
        return reinterpret_cast<VkPhysicalDeviceFeatures2*>(head);
    }


    std::vector<VkPhysicalDevice> Device::queryDevices(const VkInstance& instance) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        // Edge case, no VK devices discovered:
        if (deviceCount == 0) {
            TraceLog(Core::LogLevel::FATAL, "[VkDeviceEnumeration]: Failed to find GPUs with Vulkan support!");
        }

        // alLOC MEMORY FOR HANDLES
        std::vector<VkPhysicalDevice> devices(deviceCount);

        auto result = vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
        if (result != VkResult::VK_SUCCESS) {
            TraceLog(Core::LogLevel::FATAL, "[VkDeviceEnumeration]: Failed to query GPUs! count: {}, VKError: {}", deviceCount, string_VkResult(result));
        }

        return devices;
    }

    std::vector<VkPhysicalDevice> Device::minDeviceRequirements(VkInstance& instance, DeviceFeatureSet& reqFeatures) {
        auto featureQuery = queryDevices(instance);

        for (auto d : featureQuery) {
            d.getFeatures
        }

        // Check for required compatability, remove device from lsit if unsupported:
        for (auto device = featureQuery.begin(); device != featureQuery.end(); ) {

            auto availableFeatures = device;
        }

    }
}
