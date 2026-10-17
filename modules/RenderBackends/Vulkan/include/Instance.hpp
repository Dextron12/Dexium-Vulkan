//
// Created by ethan on 9/9/26.
//

#ifndef DEXIUM_INSTANCE_HPP
#define DEXIUM_INSTANCE_HPP

#include <vulkan/vulkan.h>

#include <core/ApplicationInfo.hpp>

#include <RenderBackends/Vulkan/include/VkExtensionResult.hpp>


// Forward declare GLFWwindow (IF USe GLFW WM)
#ifdef DX_WM_GLFW
class GLFWwindow;
#endif

namespace Dexium::Vulkan {

    // Utility helper fn to examine a VkResult and return a human-readable error-code
    std::string inspectVkResult(const VkResult& res);


    // Forward declare for Dx::Vulkan::Device
    class Device;

    // Callback function for validation layers(default)
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagBitsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallback, void* pUserData);

    class Instance {
    public:
        Instance(ApplicationInfo appInfo);

#if defined(DX_WM_GLFW)
        [[nodiscard]] VkSurfaceKHR createWindowSurface(GLFWwindow* window);
#endif

        // Validates and enabled provided validation layers, return s a vec of unsupported layer names (if any)
        std::vector<const char*> enableValidationLayers(const std::vector<const char*>& reqValdiationLayers);

        // Validates extension support and enables it if supported. NOTE: extensions can only be enabled before instance creation (Instance::createInstance)
        DX_EXT_PACK enableExtensions(const std::vector<const char*>& extNames);

        bool createInstance();
        VkInstance get() const;

        // Query available physical devices
        std::vector<VkPhysicalDevice> enumeratePhysicalDevices();



    private:
        ApplicationInfo appInfo;
        VkInstance instance = VK_NULL_HANDLE;

        std::vector<const char*> extensions; // extensions that are to be enabled at createInstance
        std::vector<VkExtensionProperties> availableEXTProperties;

        std::vector<VkLayerProperties> availableLayers; // Queried by driver
        std::vector<const char*> enabledValidationLayers; // Any succesful layer passed to `enableValidationLayers` will be added to this vec

        void enumEXTProperties();

    };
}

#endif //DEXIUM_INSTANCE_HPP
