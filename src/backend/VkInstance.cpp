//
// Created by ethan on 17/5/26.
//


// target_host = The machine which this software will run on.

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <unordered_set>
#include <vulkan/vulkan.hpp>

// Create global dispatcher (Must be doen once in a single target/.cpp file)
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE



#include <backend/VKInstance.hpp>
#include <GLFW/glfw3.h>

#include <core/Error.hpp>

#include <string_view>
#include <algorithm>

const Dexium::Core::VersionControl EngineVersion(0, 30, 0);

namespace Dexium::Vulkan {


    VkInstance::VkInstance(Core::DxApplicationInfo& DXAppInfo) {
        // Instantiate the Vk DynamicLoader:
        // Essentially determines the host OS's location of its VK dynamic lib and laods its functions into a vtable for use
        loader = std::make_unique<vk::detail::DynamicLoader>();


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
        auto vkGetInstanceProcAddr = loader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        // Check support for requested validation layers & configure debug callback for layers
        auto debugCreateInfo = enumerateLayers_CreateDebugInfo(&DXAppInfo);

        // Fetch requested extensions from GLFW
        const auto& requiredExtensions = getRequiredInstanceExtensions();

        // Check if the GLFW required extenions are supported by the Vk implementation
        auto extensionProperties = vk::enumerateInstanceExtensionProperties();

        std::unordered_set<std::string_view> availableExtensions;
        availableExtensions.reserve(extensionProperties.size());

        for (auto const& ext : extensionProperties) {
            availableExtensions.emplace(ext.extensionName);
        }

        size_t unsupportedCount = 0;
        size_t supportedCount = 0;

        for (auto const& required : requiredExtensions) {
            if (availableExtensions.find(required) != availableExtensions.end()) {
                ++supportedCount;
            } else {
                ++unsupportedCount;
                // WARN: For unsupported extension
                TraceLog(Core::LogLevel::WARN, "[VkInstance]: The requested GLFW extension: {}, is unsupported by your Vk implementation!", required);
            }
        }

        TraceLog(Core::LogLevel::TRACE, "Loading {} extensions", std::to_string(supportedCount));


        // Ref for validationLayers (after enumerateLayers_CreateDebugInfo is called, since it modifies the DXAppInfo vec)
        auto& validationLayers = DXAppInfo.requestLayerFeatures;

        // Create a Vulkan Instance
        vk::InstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
#ifdef DX_LayerFeatures
        instanceCreateInfo.pNext = &debugCreateInfo;
#endif

        auto result= vk::createInstance(&instanceCreateInfo, nullptr, &instance);
        if (result != vk::Result::eSuccess) {
            TraceLog(Core::LogLevel::FATAL, "Creating a VkInstance failed");
        }
        // register it with the global loader
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

        TraceLog(Core::LogLevel::TRACE, "Created a VkInstance");

    }

    std::vector<const char*> VkInstance::getRequiredInstanceExtensions() {
        uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Convert const char* to vector
        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Add debug mesg callback extensions if validation layers are in use
#ifdef DX_LayerFeatures
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
#endif

        TraceLog(Core::LogLevel::TRACE, "GLFW requested {} extenions", std::to_string(glfwExtensionCount));

        return extensions;
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


namespace Dexium::Vulkan {
    VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {

        // Push all messages by their type to TraceLog
        Dexium::Core::LogLevel level;

        switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
            level = Dexium::Core::LogLevel::INFO; break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            level = Dexium::Core::LogLevel::WARN; break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            level = Dexium::Core::LogLevel::ERROR; break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            level = Dexium::Core::LogLevel::TRACE; break;
        default:
            level = Dexium::Core::LogLevel::DEBUG; break;
        }

        // Pass on the severity level to logging system with formatted mesg

        // Idneitfy message type and store it
        std::string msgType = "General";

        if (type & vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
            msgType = "Validation";
        else if (type & vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
            msgType = "Performance";

        TraceLog(level, "[VK {}][Name: {}, ID: {}]: {}",msgType, pCallbackData->pMessageIdName, pCallbackData->messageIdNumber, pCallbackData->pMessage);

        // When TraceLog officially supports timestamps through its formatter, modify the formatter to output timestamp

        // Done so VK doesn't abort the call
        return VK_FALSE;
    }

    vk::DebugUtilsMessengerCreateInfoEXT enumerateLayers_CreateDebugInfo(Core::DxApplicationInfo* appInfo) {
        // Get the user-requested validation layers
        auto& validationLayers = appInfo->requestLayerFeatures;

        // Because the vec within DXAppInfo can be empty(no user-requested layers) and we expose the amcro DX_LayerFeatures,
        // we need to check if the VK_LAYER_KHRONOS_valdiation is in the vec or add it if the amcro layer is enabled:
#ifdef DX_LayerFeatures
        if (std::none_of(validationLayers.begin(), validationLayers.end(),
            [](const char* layer)
            {
                return std::string_view(layer) == "VK_LAYER_KHRONOS_validation";
            })) {

            validationLayers.push_back("VK_LAYER_KHRONOS_validation"); // Manually insert request to sue KHR valdiaiton layer
        }

        // Request available supported layers from the VK instance
        const auto availableLayers = vk::enumerateInstanceLayerProperties();

        // Check that all requested layers are supported by the VK impl
        //for (const char* reqLayer : validationLayers) {
        for (auto it = validationLayers.begin(); it != validationLayers.end(); ) {
            auto reqLayer = *it;
            if (std::none_of(availableLayers.begin(), availableLayers.end(),
                [reqLayer](const vk::LayerProperties& layer)
                {
                    return std::string_view(layer.layerName) == reqLayer;
                    //return std::strcmp(layer.layerName, reqLayer);
                })) {
                // Layer not supported. WARN:
                TraceLog(Core::LogLevel::WARN, "[VK_VALIDATION_LAYERS]: {} is an unsupported layer by your host implementation", reqLayer);
                // We pass the same vector to VkInstanceCreateInfo, so we need to remvoe any unsupported layers
                it = validationLayers.erase(it);
            } else {
                ++it; // only advance when not removing an unsupported layer
            }
        }
#endif

        // Provide debug info for the validation layers to use TraceLog
        // the free fn Dexium::Vulkan::debugCallback is a callback fn that re-routes all VK logging messages to TraceLog

         auto debugMessenger = vk::DebugUtilsMessengerCreateInfoEXT{
            {},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &Dexium::Vulkan::debugCallback // THe TraceLog callback fn
        };

        return debugMessenger;
    }

    const std::vector<const char*> enumerateInstanceExtensions() {
        // Check the required extensions from GLFW(no need for the window interface to do so)
        auto& requiredExtensions = VkInstance::getRequiredInstanceExtensions();

        // Get all supported extenions by the Vk impl
        auto extensionProperties = vk::enumerateInstanceExtensionProperties();

        std::unordered_set<std::string_view> availableExtensions;
        availableExtensions.reserve(extensionProperties.size());

        for (auto const& ext : extensionProperties) {
            availableExtensions.emplace(ext.extensionName);
        }
        size_t unsupportedCount = 0;
        size_t supportedCount = 0;

        //for (auto const& required : requiredExtensions) {
        for (auto it = requiredExtensions.begin(); it != requiredExtensions.end(); ) {
            auto required = *it;
            if (availableExtensions.find(required) != availableExtensions.end()) {
                ++supportedCount;
                //Update iter when nothing to erase.... this feels really dodgy
                ++it;
            } else {
                ++unsupportedCount;
                TraceLog(Core::LogLevel::WARN, "[VkInstance]: The requested extension: {} is unsupported by the Vk implementation", required);
                it = requiredExtensions.erase(it);
            }
        }

        TraceLog(Core::LogLevel::TRACE, "Loading {} extensions", std::to_string(supportedCount));
    }
}
