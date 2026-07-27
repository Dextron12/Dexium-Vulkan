//
// Created by ethan on 12/6/26.
//

#include <backend/Device.hpp>

#include <core/Error.hpp>

#include <vulkan/vk_enum_string_helper.h> // Provides string_enum(enum value), to stringify pesky Vk enums!

#include <vulkan/vulkan.h>

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


    /*std::vector<VkPhysicalDevice> Device::queryDevices(const VkInstance& instance) {
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
    }*/

    std::vector<VkPhysicalDevice> Device::minDeviceRequirements(VkInstance& instance, DeviceFeatureSet& reqFeatures) {
        //auto featureQuery = queryDevices(instance);

        auto deviceQuery = queryDevices(instance);

        if (deviceQuery.empty()) {
            TraceLog(Core::LogLevel::WARN, "[VkDevice]: No physical devices detected!");
            return std::vector<VkPhysicalDevice>();
        }


        for (auto device = deviceQuery.begin(); device != deviceQuery.end(); ) {
            DeviceFeatureSet deviceSupport{};

            // My love-hate relationship for Vulkan si best described by the implementation of StructureChains...
            deviceSupport.vulkan14Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;

            // Define & link 1_3_FEATURES to 1_4 features
            deviceSupport.vulkan13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
            deviceSupport.vulkan13Features.pNext = &deviceSupport.vulkan14Features;

            // Define & link 1_2 FEATURES to 1_3 FEATURES
            deviceSupport.vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
            deviceSupport.vulkan12Features.pNext = &deviceSupport.vulkan13Features;

            // Define & link 1_1 FEATURES to 1_2 FEATURES
            deviceSupport.vulkan11Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
            deviceSupport.vulkan11Features.pNext = &deviceSupport.vulkan12Features;

            // Define & link 1_0 features to 1_2 features
            deviceSupport.vulkanFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
            deviceSupport.vulkanFeatures2.pNext = &deviceSupport.vulkan11Features;

            // Query the device supported features from Vk ( WIll populate the rest of the structure chain)
            vkGetPhysicalDeviceFeatures2(*device, &deviceSupport.vulkanFeatures2);

            bool supports10Features, supports11Features, supports12Features, supports13Features, supports14Features = false;

            auto t = deviceSupport.vulkanFeatures2.features;


            // Iterate over the user-required features and ensure they are supported
            auto const uFeatureChain = reinterpret_cast<VkBool32 const*>(&deviceSupport.vulkanFeatures2);
            size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

            for (size_t i = 0; i < count; ++i) {
                if (uFeatureChain[i]) {}
            }
        }

        auto const* flags = reinterpret_cast<VkBool32 const*>(&reqFeatures.vulkanFeatures2);
        size_t count = sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32);

        for (size_t i = 0; i < count; ++i) {
            if (flags[i]) {
                // Feature enabled
            }
        }


        // Cannot provide any real enumeration or tests on VkPhysicalDevice and similar structures, they are a C API that doesnt expose its members
        // Need dynamic reflection system (may be able to pull it off CXX26 reflection) to access member types



    }
}
