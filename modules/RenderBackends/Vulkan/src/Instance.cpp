//
// Created by ethan on 11/9/26.
//

#include <RenderBackends/Vulkan/include/Instance.hpp>

#ifdef DX_PLATFORM_GLFW
#include <GLFW/glfw3.h>
#endif

#include <util/Logger.hpp>
#include <core/ServiceLocator.hpp>

#include <algorithm>

namespace Dexium::Vulkan {

    std::string inspectVkResult(const VkResult& res) {
        std::string err;

        switch (res) {
        case VK_SUCCESS:
            err = "VK_SUCCESS";
            break;
        case VK_NOT_READY:
            err = "VK_NOT_READY";
            break;
        case VK_TIMEOUT:
            err = "VK_TIMEOUT";
            break;
        case VK_EVENT_SET:
            err = "VK_EVENT_SET";
            break;
        case VK_EVENT_RESET:
            err = "VK_EVENT_RESET";
            break;
        case VK_INCOMPLETE:
            err = "VK_INCOMPLETE";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            err = "VK_ERROR_OUT_OF_HOST_MEMORY";
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            err = "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            err = "VK_ERROR_INITIALIZATION_FAILED";
            break;
        case VK_ERROR_DEVICE_LOST:
            err = "VK_ERROR_DEVICE_LOST";
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            err = "VK_ERROR_MEMORY_MAP_FAILED";
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            err = "VK_ERROR_LAYER_NOT_PRESENT";
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            err = "VK_ERROR_EXTENSION_NOT_PRESENT";
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            err = "VK_ERROR_FEATURE_NOT_PRESENT";
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            err = "VK_ERROR_INCOMPATIBLE_DRIVER";
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            err = "VK_ERROR_TOO_MANY_OBJECTS";
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            err = "VK_ERROR_FORMAT_NOT_SUPPORTED";
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            err = "VK_ERROR_FRAGMENTED_POOL";
            break;
        case VK_ERROR_UNKNOWN:
            err = "VK_ERROR_UNKNOWN";
            break;
        case VK_ERROR_VALIDATION_FAILED:
            err = "VK_ERROR_VALIDATION_FAILED";
            break;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            err = "VK_ERROR_OUT_OF_POOL_MEMORY";
            break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            err = "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            break;
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            err = "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
            break;
        case VK_ERROR_FRAGMENTATION:
            err = "VK_ERROR_FRAGMENTATION";
            break;
        case VK_PIPELINE_COMPILE_REQUIRED:
            err = "VK_PIPELINE_COMPILE_REQUIRED";
            break;
        case VK_ERROR_NOT_PERMITTED:
            err = "VK_ERROR_NOT_PERMITTED";
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            err = "VK_ERROR_SURFACE_LOST_KHR";
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            err = "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            break;
        case VK_SUBOPTIMAL_KHR:
            err = "VK_SUBOPTIMAL_KHR";
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            err = "VK_ERROR_OUT_OF_DATE_KHR";
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            err = "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            err = "VK_ERROR_INVALID_SHADER_NV";
            break;
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            err = "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
            break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            err = "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
            break;
        case VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT:
            err = "VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT";
            break;
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            err = "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
            break;
        case VK_THREAD_IDLE_KHR:
            err = "VK_THREAD_IDLE_KHR";
            break;
        case VK_THREAD_DONE_KHR:
            err = "VK_THREAD_DONE_KHR";
            break;
        case VK_OPERATION_DEFERRED_KHR:
            err = "VK_OPERATION_DEFERRED_KHR";
            break;
        case VK_OPERATION_NOT_DEFERRED_KHR:
            err = "VK_OPERATION_NOT_DEFERRED_KHR";
            break;
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            err = "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
            break;
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            err = "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
            break;
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT:
            err = "VK_INCOMPATIBLE_SHADER_BINARY_EXT";
            break;
        case VK_PIPELINE_BINARY_MISSING_KHR:
            err = "VK_PIPELINE_BINARY_MISSING_KHR";
            break;
        case VK_ERROR_NOT_ENOUGH_SPACE_KHR:
            err = "VK_ERROR_NOT_ENOUGH_SPACE_KHR";
            break;
        case VK_RESULT_MAX_ENUM:
            err = "VK_RESULT_MAX_ENUM";
            break;
        }

        return err;
    }

    VKAPI_ATTR VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallback, void* pUserData) {
        // Get Logger from service lcoator
        TraceLog(LogLevel::TRACE, "Validation Layer: type{}\nmsg {}\n", std::to_string(type), pCallback->pMessage);

        return VK_FALSE;
    }

    Instance::Instance(ApplicationInfo appInfo)
        : appInfo(appInfo) {

        // Query extensions
        uint32_t extCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        //resize vec
        availableEXTProperties.resize(extCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, availableEXTProperties.data());

        // Query layer support
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        //Resize vec:
        availableLayers.resize(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    }
    DX_EXT_PACK Instance::enableExtensions(const std::vector<const char*>& extNames) {
        DX_EXT_PACK res;
        // Validate each extension from host driver extension (Instance::availableEXTProperties)
        for (const auto& extName : extNames) {
            bool found = false;

            for (const VkExtensionProperties& ext : availableEXTProperties) {
                if (strcmp(ext.extensionName, extName) == 0) {
                    res.noEnabledEXT++;
                    res.enabledEXT.push_back(ext.extensionName);

                    // EXT supported by host VK driver, add to extensions vec to be enabled at instance creation
                    extensions.push_back(ext.extensionName);

                    found = true;
                }
            }

            if (!found) {
                res.unsupportedEXT.push_back(extName);
                res.noFailedEXT++;
            }
        }

        return res;
    }

    std::vector<const char*> Instance::enableValidationLayers(const std::vector<const char*>& requestedLayers) {

        // Ensure the specified layers are supported by the driver
        std::vector<const char*> unsupportedLayers;

        for (const char* layerName : requestedLayers) {
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


#ifdef DX_PLATFORM_GLFW
    VkSurfaceKHR Instance::createWindowSurface(GLFWwindow* window) {
        VkSurfaceKHR surface = VK_NULL_HANDLE;

        if (!window) {
            TraceLog(LogLevel::WARN, "Window is nullptr on surface creation");
            return surface;
        }

        // Check that the instance is ready
        if (instance == VK_NULL_HANDLE) {
            TraceLog(LogLevel::WARN, "Cannot create a window surface with uninitialised VKInstance");
            return surface;
        }

        VkResult res;
        res = glfwCreateWindowSurface(instance, window, nullptr, &surface);
        if (res != VK_SUCCESS) {
            TraceLog(LogLevel::ERROR, "Failed to create a VkSurface for GLFW context\nReason: {}", inspectVkResult(res));
            return surface;
        }

        return surface;
    }

#endif


    std::vector<VkPhysicalDevice> Instance::enumeratePhysicalDevices() {
        //local storage
        uint32_t deviceCount = 0;
        std::vector<VkPhysicalDevice> devices;

        // Check for a valid instance
        if (instance == VK_NULL_HANDLE) {
            TraceLog(LogLevel::ERROR, "Cannot instantiate GPU's without a valid VkInstance");
            return devices;
        }

        // First call: Check for device count
        if (vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr) != VK_SUCCESS) {
            TraceLog(LogLevel::ERROR, "Failed to instantiate GPU devices");
            return devices;
        }

        //Check that at least one device is discovered
        if (deviceCount == 0) {
            TraceLog(LogLevel::WARN, "No GPU with Vulkan support detected!");
            return devices;
        }

        // Allocate space for the device handles (Vk wont do this for us)
        devices.resize(deviceCount);

        // Second call: retrieve device handles
        if (vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()) != VK_SUCCESS) {
            TraceLog(LogLevel::ERROR, "Failed to capture GPU device handles");
            devices.clear();
            return devices;
        }

        return devices;
    }
}
