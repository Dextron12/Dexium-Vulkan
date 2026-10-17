//
// Created by ethan on 3/9/26.
//


#include <Device.hpp>

#include <util/Logger.hpp>

#include <iterator> // For std::back_inserter

namespace Dexium::Vulkan {
    Device::Device(VkPhysicalDevice pDeviceHandle) {
        // Check that device handle is valid
        if (pDeviceHandle == VK_NULL_HANDLE) {
            //TraceLog(LogLevel::WARN, "Cannot create a device with NULL pDevice handle");
            // Device creation SHOULD NOT log a message here, instead leave it to the usr, incase a null handle is required
            return;
        }

        // Store the handle
        m_pDevice = pDeviceHandle;

        // Query device properties
        properties = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(m_pDevice, &properties);

        // Query extensiuons
        extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(m_pDevice, nullptr, &extensionCount, nullptr); // Returns a VkResult, should check it
        // Realloc extensions vec
        availableExtensions.resize(extensionCount);
        // Set available extensions
        vkEnumerateDeviceExtensionProperties(m_pDevice, nullptr, &extensionCount, availableExtensions.data());

        // Query queuefamily properties
        queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_pDevice, &queueFamilyCount, nullptr);
        // Realloc
        queueFamilyProperties.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_pDevice, &queueFamilyCount, queueFamilyProperties.data());
    }

    VkPhysicalDevice Device::getPDevice() const {
        return m_pDevice;
    }

    VkDevice Device::get() const {
        return m_Device;
    }

    std::vector<const char*> Device::checkExtensionSupport(const std::vector<const char*>& requiredExtensions) {
        // Check that `availableExtensions` is populated
        if (availableExtensions.empty()) {
            TraceLog(LogLevel::ERROR, "Cannot check device extension compatibility, no available extensions detected for {}", properties.deviceName);
            return requiredExtensions; // Failed to check extensions, return all extensions so program knows that it failed
        }

        // Ensure that requiredExtensions have content
        if (requiredExtensions.empty()) {
            TraceLog(LogLevel::ERROR, "No required extensions given for extension support on device: {}", properties.deviceName);
            return requiredExtensions;
        }

        std::vector<const char*> unsupportedExtensions{};

        std::ranges::copy_if(requiredExtensions, std::back_inserter(unsupportedExtensions),
            [&](const char* required)
            {
                return std::ranges::none_of(availableExtensions,
                    [&](const VkExtensionProperties& available)
                    {
                        return std::strcmp(required, available.extensionName) == 0; // Using strcmp or std::string_view here wont matter too much, compiler will try optimize either way
                    });
            });

        // Return unsupported extensions
        return unsupportedExtensions;
    }


    bool Device::supportsGraphicsQueue() const {
        // Check that a graphics queue is available
        return std::ranges::any_of(queueFamilyProperties,
            [](auto const& qfp)
            {
                return !!(qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT);
            });
    }
}
