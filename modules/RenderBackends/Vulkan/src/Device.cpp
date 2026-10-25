//
// Created by ethan on 3/9/26.
//


#include <RenderBackends/Vulkan/include/Device.hpp>

#include <util/Logger.hpp>

namespace Dexium::Vulkan {
    VkBool32 QueueFamilyInfo::queryPresentationSupport(VkPhysicalDevice pDevice, VkSurfaceKHR renderSurface) const {
        VkBool32 presentSupport = false;

        vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, index, renderSurface, &presentSupport);

        return presentSupport;
    }

    void physicalDevice::queryPhysicalDeviceQfProperties() {
        if (queueFamilyProperties.empty() || queueFamilyCount == 0) {
            // Query pDevice qfProeprties
            uint32_t qfCount = 0;

            vkGetPhysicalDeviceQueueFamilyProperties(m_Device, &qfCount, nullptr);
            queueFamilyProperties.resize(qfCount);
            vkGetPhysicalDeviceQueueFamilyProperties(m_Device, &qfCount, queueFamilyProperties.data());

            queueFamilyCount = qfCount;
        }
    }

    physicalDevice::physicalDevice(VkPhysicalDevice pDeviceHandle) {
        // Check that device handle is valid
        if (pDeviceHandle == VK_NULL_HANDLE) {
            //TraceLog(LogLevel::WARN, "Cannot create a device with NULL pDevice handle");
            // Device creation SHOULD NOT log a message here, instead leave it to the usr, incase a null handle is required
            return;
        }

        // Store the handle
        m_Device = pDeviceHandle;

        // Query device properties
        properties = VkPhysicalDeviceProperties{};
        vkGetPhysicalDeviceProperties(m_Device, &properties);

        // Query extensiuons
        extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(m_Device, nullptr, &extensionCount, nullptr); // Returns a VkResult, should check it
        // Realloc extensions vec
        availableExtensions.resize(extensionCount);
        // Set available extensions
        vkEnumerateDeviceExtensionProperties(m_Device, nullptr, &extensionCount, availableExtensions.data());

        // Query queuefamily properties
        queryPhysicalDeviceQfProperties();
    }

    VkPhysicalDevice physicalDevice::get() const {
        return m_Device;
    }

    DX_EXT_PACK physicalDevice::checkExtensionSupport(const std::vector<const char*>& requiredExtensions) {
        // Check that `availableExtensions` contain data
        if (requiredExtensions.empty()) {
            return {.noFailedEXT = requiredExtensions.size(), .unsupportedEXT = requiredExtensions};
        }

        // Iterate over extensions to test
        DX_EXT_PACK res;

        for (const auto& ext : requiredExtensions) {
            bool found = false;

            // Iterate over the available device extensions, looking for a match
            for (const VkExtensionProperties& suppEXT : availableExtensions) {
                if (strcmp(suppEXT.extensionName, ext) == 0) {
                    res.noEnabledEXT++;
                    res.enabledEXT.push_back(suppEXT.extensionName);

                    found = true;
                }
            }

            if (!found) {
                res.unsupportedEXT.push_back(ext);
                res.noFailedEXT++;
            }
        }

        return res;
    }


    std::vector<QueueFamilyInfo> physicalDevice::queryQueueFamilies() {

        // Check if qf info quered:
        if (queueFamilyProperties.empty()) {
            queryPhysicalDeviceQfProperties();
        }

        std::vector<QueueFamilyInfo> res;
        if (queueFamilyCount != 0) {
            // Resize vect to match qfp
            res.resize(queueFamilyCount);
        }

        // Iterate over qf's
        for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++) {
            const auto& qfp = queueFamilyProperties[qfpIndex];

            auto& input = res[qfpIndex];

            // Query graphics queue support:
            if ( (qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT ) {
                input.capabilities |= VK_QUEUE_GRAPHICS_BIT;
                input.supportsGraphics = true;
            }

            // Query compute support
            if ( (qfp.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT ) {
                input.capabilities |= VK_QUEUE_COMPUTE_BIT;
                input.supportsCompute = true;
            }

            // SET the MAX number of supported queues for the family
            input.queueCount = qfp.queueCount;
        }

        return res;
    }

    QueueFamilyInfo physicalDevice::getDefaultQueueFamily(VkSurfaceKHR renderSurface) {
        const auto& qFamilies = queryQueueFamilies();

        std::vector<std::pair<int, size_t>> rankings;

        for (size_t i = 0; i < qFamilies.size(); i++) {
            const auto& qf = qFamilies[i];
            int score = 0;

            if (qf.supportsGraphics) score += 50;
            if (qf.supportsCompute) score += 20;

            if (qf.queryPresentationSupport(m_Device, renderSurface)) score += 50;

            if ( (qf.capabilities & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT ) score += 10;

            rankings.emplace_back(score, i);
        }

        // Sort rankings largest to smallet
        std::ranges::sort(rankings, [](const auto& a, const auto& b)
        {
            return a.first > b.first;
        });

        // First item in rankings will be msot desired qf
        auto best = qFamilies[rankings[0].second];
        best.createQueues = 1; // Dexium does it best to work with one queue and hence its the default setting
        // This default behaviour can be overriden directly after recieving QueueFamilyInfo from this fn, or by using the sister fn requestQueueFamily()

        return best;
    }

    QueueFamilyInfo physicalDevice::requestQueueFamily(const QueueRequest& request, VkSurfaceKHR renderSurface) {
        // Query the queueFamilies
        const auto& qFamilies = queryQueueFamilies();

        std::vector<std::pair<int, size_t>> rankings;

        for (size_t i = 0; i < qFamilies.size(); i++) {
            const auto& qf = qFamilies[i];
            int score = 100;

            // The below attribs detract from the qf score as they aren't alligned with what has been requested
            // Check: requested queues are greater than what the family can provide
            if (request.queueCount > qf.queueCount) score -= 50;

            // Check: graphics queue support
            if (request.requireGraphics && !qf.supportsGraphics) score -= 50;

            // Check: compute queue support
            if (request.requireCompute && !qf.supportsCompute) score -= 30;

            // Check: Present queue support for the provided renderSurface
            if (request.requirePresent && !qf.queryPresentationSupport(m_Device, renderSurface)) score -= 50;

            // emplace ranking
            rankings.emplace_back(score, i);
        }

        // Sort: Biggest to smallest
        std::ranges::sort(rankings, [](const auto& a, const auto& b)
        {
            return a.first > b.first;
        });

        auto mostSuitable = qFamilies[rankings[0].second];

        // Request number of queues to create upto MAX supported for this qf
        if (request.queueCount <= mostSuitable.queueCount) {
            mostSuitable.createQueues = request.queueCount;
        } else {
            // User-requested queue count exceeds the number of available queues for this family
            mostSuitable.createQueues = 1; // Set to back to the engine default
        }

        return mostSuitable;
    }
}
