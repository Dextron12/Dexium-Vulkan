//
// Created by ethan on 11/9/26.
//


#include <Instance.hpp>

#include <util/Logger.hpp>
#include <core/ServiceLocator.hpp>

#include <algorithm>

#if defined(DX_WM_GLFW)
#include <GLFW/glfw3.h>
#endif

namespace Dexium::Vulkan {

    VKAPI_ATTR VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallback, void* pUserData) {
        // Get Logger from service lcoator
        TraceLog(LogLevel::TRACE, "Validation Layer: type{}\nmsg {}\n", std::to_string(type), pCallback->pMessage);

        return VK_FALSE;
    }

    Instance::Instance(ApplicationInfo appInfo)
        : appInfo(appInfo) {}

    std::vector<const char*> Instance::checkGLFWExtensions() {
        uint32_t glfwEXTCount = 0;
        auto glfwEXT = glfwGetRequiredInstanceExtensions(&glfwEXTCount);

        // Query Vk Driver for all supported extensions
        if (availableEXTProperties.empty()) enumEXTProperties();

        // Check that the GLFW extensions are suppoprted by the isntance/driver
        std::vector<const char*> unsupportedEXT;
        for (uint32_t i = 0; i < glfwEXTCount; i++) {
            if (std::ranges::none_of(availableEXTProperties, [required = glfwEXT[i]](const VkExtensionProperties& ext)
            {
                return strcmp(ext.extensionName, required) == 0;
            })) {
                unsupportedEXT.push_back(glfwEXT[i]);
            }
        }

        return unsupportedEXT;
    }

    std::vector<const char*> Instance::enableValidationLayers(const std::vector<const char*>& reqValdiationLayers) {
        // Query available validations layers from the Vk driver
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        availableLayers.resize(layerCount);

        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Ensure the specified layers are supported by the driver
        std::vector<const char*> unsupportedLayers;

        for (const char* layerName : reqValdiationLayers) {
            if (std::ranges::none_of(availableLayers, [layerName, &eLayers = enabledValidationLayers](const VkLayerProperties& layer)
            {
                eLayers.push_back(layerName);
                return strcmp(layer.layerName, layerName) == 0;
            })) {
                unsupportedLayers.push_back(layerName);
            }
        }

        return unsupportedLayers;
    }


    void Instance::enumEXTProperties() {
        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);

        //Resize vec
        availableEXTProperties.resize(extCount);

        //ALlocate
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, availableEXTProperties.data());

    }

    bool Instance::createInstance() {
        if (!enabledValidationLayers.empty()) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,

            .messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,

            .messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,

            .pfnUserCallback = debugCallback
        };

        VkInstanceCreateInfo instanceCreateInfo{
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = enabledValidationLayers.empty() ? nullptr : &debugCreateInfo,

            .pApplicationInfo = &appInfo.appInfo,

            .enabledLayerCount =
                static_cast<uint32_t>(enabledValidationLayers.size()),
            .ppEnabledLayerNames = enabledValidationLayers.data(),

            .enabledExtensionCount =
                static_cast<uint32_t>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
            TraceLog(LogLevel::FATAL, "Failed to create a Vulkan instance!");
            instance = VK_NULL_HANDLE;
            return false;
        }

        return true;
    }


    VkInstance Instance::get() const {
        return instance;
    }
}
