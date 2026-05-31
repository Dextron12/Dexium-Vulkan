//
// Created by ethan on 17/5/26.
//


// target_host = The machine which this software will run on.

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

// Create global dispatcher (Must be doen once in a single target/.cpp file)
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE



#include <backend/VKInstance.hpp>

const Dexium::Core::VersionControl EngineVersion(0, 30, 0);

namespace Dexium::Vulkan {
    VkInstance::VkInstance(const Core::DxApplicationInfo& DXAppInfo) {
        // Create the ApplicationInfo context
        appInfo = vk::ApplicationInfo(
            DXAppInfo.appTitle.empty() ? "Dexium Application" : DXAppInfo.appTitle.c_str(),
            VK_MAKE_VERSION(DXAppInfo.appVersion.major, DXAppInfo.appVersion.minor, DXAppInfo.appVersion.patch),
            "Dexium Framework",
            VK_MAKE_VERSION(EngineVersion.major, EngineVersion.minor, EngineVersion.patch),
            DXAppInfo.VkApiVersion
            );

        // Fetch the Vulkan API version installed on target_host
        VkDynamicVersion = Core::VersionControl{
        VK_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),
        VK_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)};

        // Dynamically load Vulkan API (per machine setup)
        // Get ProcAddress of vkGetInstanceProcAddr
        auto vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        // Create a Vulkan Instance
        vk::InstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.pApplicationInfo = &appInfo;

        //instance = vk::createInstance(instanceCreateInfo);
        auto result = vk::createInstance(&instanceCreateInfo, nullptr, &instance);

        // register it with the global loader
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

        /*

        // Check if the requested layers are supported by the implementation
#ifdef DX_LayerFeatures
        auto layerProperties = context.enumerateInstanceLayerProperties();
        if (std::any_of(requestLayerFeatures.begin(), requestLayerFeatures.end(),
            [&layerProperties](auto const& requiredLayer)
            {
                return std::none_of(layerProperties.begin(), layerProperties.end(),
                    [requiredLayer](auto const& layerProperty)
                    {
                        return std::strcmp(layerProperty.layerName, requiredLayer) == 0;
                    });
            })) {
            // Raise FATAL error here
            fmt::printf(stderr, fmt::format)
        }

#endif
*/


    }

    VkInstance::~VkInstance() {
        destroyInstance();
    }

    void VkInstance::destroyInstance() {
        // First (top-most) VK object should be destroyed last!!

        // Destroy the Instance
        instance.destroy();
    }
}
