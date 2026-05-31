//
// Created by ethan on 17/5/26.
//

#ifndef DEXIUM_VKINSTANCE_HPP
#define DEXIUM_VKINSTANCE_HPP

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
    class VkInstance {
    public:
        VkInstance(const Core::DxApplicationInfo& DXAppInfo);
        ~VkInstance();

        void destroyInstance();

        Core::VersionControl VkDynamicVersion;

        vk::detail::DynamicLoader loader;

        vk::Instance instance = nullptr;

        vk::SurfaceKHR windowSurface = nullptr;

    private:
        vk::ApplicationInfo appInfo;
    };
}

#endif //DEXIUM_VKINSTANCE_HPP