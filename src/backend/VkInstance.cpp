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

        // DebugInfo to store any vlaidation layers & accessor to the debug callback
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        // May be empty if layers are NOT in use. ALso used to parse information about use of layers to isntance creation
        auto& validationLayers = DXAppInfo.requestLayerFeatures;
#ifdef DX_LayerFeatures
        // Check if validation layers are enabled (if so, ensure the KHRONOS valdiaiton layer is added to the vector)
        if (std::none_of(validationLayers.begin(), validationLayers.end(),
            [](const char* layer)
            {
                return std::string_view(layer) == "VK_LAYER_KHRONOS_validation"; // Check if this exists within the vector
            })) {

            validationLayers.push_back("VK_LAYER_KHRONOS_validation"); // Layer NOT found, inserting it!
        }

        // Beg Vk for supoported valdiation layers
        const auto availableLayers = vk::enumerateInstanceLayerProperties();

        // Check that requested layers are supported by HOSt impllementation
        for (const char* reqLayer : validationLayers) {
            if (std::none_of(availableLayers.begin(), availableLayers.end(),
                [reqLayer](const vk::LayerProperties& layer)
                {
                    return std::string_view(layer.layerName) == reqLayer;
                })) {
                // Layer not supported(WARN)
                TraceLog(Core::LogLevel::WARN, "[VK_VALIDATION_LAYERS]: {} is not a supported layer in the current VK-Implementation. It will be ignored", reqLayer);
            }
        }

        // Populate the debug info or validation layers
        debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT{
            {},
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
            &Dexium::Vulkan::debugCallback
        };


#endif

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

    VkInstance::~VkInstance() {
        destroyInstance();
    }

    void VkInstance::destroyInstance() {
        // First (top-most) VK object should be destroyed last!!

        // Destroy the Instance
        instance.destroy();
    }
}


// The Vk debug fn used with VK valiation layers
#if defined (DX_LayerFeatures)
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
}
#endif
