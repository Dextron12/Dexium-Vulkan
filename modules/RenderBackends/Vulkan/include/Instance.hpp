//
// Created by ethan on 9/9/26.
//

#ifndef DEXIUM_INSTANCE_HPP
#define DEXIUM_INSTANCE_HPP

#include <vulkan/vulkan.h>

#include <core/ApplicationInfo.hpp>

#include <vector>

namespace Dexium::Vulkan {


    // Callback function for validation layers(default)
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagBitsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallback, void* pUserData);


    class Instance {
    public:
        Instance(ApplicationInfo appInfo);

#if defined(DX_WM_GLFW)
        // Checks GLFW surface extension requirements with Vulkan driver, returns a vec of unsupported extensions(if any)
        std::vector<const char*> checkGLFWExtensions();
#endif

        // Validates and enabled provided validation layers, return s a vec of unsupported layer names (if any)
        std::vector<const char*> enableValidationLayers(const std::vector<const char*>& reqValdiationLayers);

        bool createInstance();
        VkInstance get() const;

    private:
        ApplicationInfo appInfo;
        VkInstance instance = VK_NULL_HANDLE;

        std::vector<const char*> extensions;
        std::vector<VkExtensionProperties> availableEXTProperties;

        std::vector<VkLayerProperties> availableLayers; // Queried by driver
        std::vector<const char*> enabledValidationLayers; // Any succesful layer passed to `enableValidationLayers` will be added to this vec

        void enumEXTProperties();

    };
}

#endif //DEXIUM_INSTANCE_HPP
