//
// Created by ethan on 17/5/26.
//

#ifndef DEXIUM_VKINSTANCE_HPP
#define DEXIUM_VKINSTANCE_HPP

#include <vulkan/vulkan.hpp>

#include <memory>


// The Vk debug fn used with VK valiation layers
#if defined (DX_LayerFeatures)
namespace Dexium::Vulkan {
    VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
}
#endif

namespace Dexium::Core {

    struct VersionControl {
        int major = 0;
        int minor = 0;
        int patch = 0;

        VersionControl(int major, int minor, int patch) : major(major), minor(minor), patch(patch) {}
        VersionControl() : major(0), minor(0), patch(0) {}
    };

    // Provide application meta-data to the runtime engine
    struct DxApplicationInfo {
        std::string appTitle;
        VersionControl appVersion;

        // The design window width and height to be used on window init
        int windowWidth, windowHeight;

        // THe VK version to use (1.1 - 1.4+) Engine only tested on V1.4
        uint32_t VkApiVersion = vk::ApiVersion14;

        // OPTIONAL validation layers (If library was compiled for validation layer support)
#ifdef DX_LayerFeatures
        std::vector<const char*> requestLayerFeatures;
        // End-user can ask for a layer to be implemented, but if VK doesnt support it, it wont be implemented
#endif
    };
}

namespace Dexium::Vulkan {

    //class VkSurfaceStructure

    class VkInstance {
    public:
        VkInstance(Core::DxApplicationInfo& DXAppInfo);
        ~VkInstance();

        // Need to move into dtor to avoid double frees
        void destroyInstance();

        // Gets the required instance extensions from GLFW
        static std::vector<const char*> getRequiredInstanceExtensions();

        Core::VersionControl VkDynamicVersion;

        // Needs to be heap-allocated so memory stays in one place
        // Newver versions also explicitly delete copy & move operators on this struct
        std::unique_ptr<vk::detail::DynamicLoader> loader = nullptr;

        vk::Instance instance = nullptr;

        vk::SurfaceKHR windowSurface = nullptr;

    private:
        vk::ApplicationInfo appInfo;
    };

    // Checks if the Vk implementation offers the suer-requested layers & configures debug information for the engine
    vk::DebugUtilsMessengerCreateInfoEXT enumerateLayers_CreateDebugInfo(Core::DxApplicationInfo* appInfo);

    // Checks if the Vk implementation can support all requested extensions from GLFW
    const std::vector<const char*> enumerateInstanceExtensions();
}

#endif //DEXIUM_VKINSTANCE_HPP