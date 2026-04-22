#include <iostream>
#include <memory>


#include <vulkan/vulkan_raii.hpp> // Uses RAII symantics and lifetime management :)
#include <GLFW/glfw3.h>

#include <Dexium.hpp>
#include <oneapi/tbb/detail/_task.h>

#ifdef NDEBUG
        constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true; // Should REALLY rename to 'enabled...'
#endif

vk::raii::Context context;
vk::raii::Instance instance = nullptr;

std::unique_ptr<Dexium::AppEngine> engineAPI = nullptr;

std::vector<const char*> getRequiredInstanceExtensions() {
    uint32_t glfwExtensionCount = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    // Add Debug msg callback extensions (If Valdiation layers are enabled)
    if (enableValidationLayers) {
        extensions.push_back(vk::EXTDebugUtilsExtensionName);
        // NOTE: vk::EXTDebugUtilsextensionName literally evals to "VK_EXT_debug_utils" but we use the macro to avoid typos
    }
    return extensions;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
    vk::DebugUtilsMessageTypeFlagsEXT type,
    const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    std::cerr << "Validation layer: type " << to_string(type) << " msg: " << pCallbackData->pMessage << std::endl;

    return vk::False;
}

vk::raii::DebugUtilsMessengerEXT debugMessenger = nullptr;

void setupDebugMessenger() {
    if (!enableValidationLayers) return;

    vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
    vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
        vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

    vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
    {},
    severityFlags,
    messageTypeFlags,
    &debugCallback};

    debugMessenger = instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
}



class Game : public Dexium::Core::ApplicationLayer {
public:

    int count = 0;

    vk::raii::PhysicalDevice physicalDevice = nullptr; // PhysicalDevice that was selected
    vk::raii::Device device = nullptr; // The Logical device (A HW/Physical device can have multiple Logical devices)

    vk::raii::Queue graphicsQueue = nullptr; // Handle to the graphicsQueue created by the logicalDevice

    vk::raii::SurfaceKHR surface = nullptr; // The Window Surfasce that Vulkan interacts with

    void onInit() override {

        vk::ApplicationInfo appInfo(
        "Hello Triangle",
        VK_MAKE_VERSION(1,0,0),
        "Dexium Framework",
        VK_MAKE_VERSION(1,0,0),
        vk::ApiVersion14);

        // Apply validation layers
        // Check if all requested layers are available
        std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        // Get the required layers
        std::vector<char const*> requiredLayers;
        if (enableValidationLayers) {
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());
        }
        // Check if the required layers are supported by the Vulkan implementation
        auto layerProperties = context.enumerateInstanceLayerProperties();
        if (std::any_of(requiredLayers.begin(), requiredLayers.end(),
            [&layerProperties](auto const& requiredLayer)
            {
                return std::none_of(layerProperties.begin(), layerProperties.end(),
                    [requiredLayer](auto const& layerProperty)
                    {
                        return std::strcmp(layerProperty.layerName, requiredLayer) == 0;
                    });
            })) {
            throw std::runtime_error("One or more required layers are not supported!");
        }

        // Get the required extensions
        auto requiredExtensions = getRequiredInstanceExtensions();

        // Check if the required extensions are supported by the Vulkan implementation
        auto extensionProperties = context.enumerateInstanceExtensionProperties();
        auto unsupportedPropertyIt = std::find_if(requiredExtensions.begin(), requiredExtensions.end(),
            [&extensionProperties](auto const &requiredExtension){
                return std::none_of(extensionProperties.begin(), extensionProperties.end(),
                    [requiredExtension](auto const& extensionProperty) {
                        return strcmp(extensionProperty.extensionName, requiredExtension) == 0;
                    }
                );
            }
        );
        if (unsupportedPropertyIt != requiredExtensions.end()) {
            throw std::runtime_error("Required extension not supported: " + std::string(*unsupportedPropertyIt));
        }

        // Create DebugInfo
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        if (enableValidationLayers) {
            debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT{
            {},
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                &debugCallback
            };
        }

        vk::InstanceCreateInfo createInfo{
        {},
        &appInfo,
        static_cast<uint32_t>(requiredLayers.size()),
        requiredLayers.data(), static_cast<uint32_t>(requiredExtensions.size()),
        requiredExtensions.data()};

        if (enableValidationLayers) {
            createInfo.pNext = &debugCreateInfo;
        }

        /*try {
            instance = vk::raii::Instance(context, createInfo);
        } catch (const vk::SystemError& err) {
            std::cerr << "Vulkan error: " << err.what() << std::endl;
        }*/
        instance = vk::raii::Instance(context, createInfo); // VK Instance created here

        // Create a window surface directlya fter a Vk isntance,a s the surface can influence device decisions
        VkSurfaceKHR _surface; // Temp surface var (Part of C API and glfw onlya ccepts the C API, but we promote it to the Raii wrapper)
        if (glfwCreateWindowSurface(*instance, engineAPI->getWindowCtx()->window, nullptr, &_surface) != 0) {
            throw std::runtime_error("failed to create surface!");
        }
        surface = vk::raii::SurfaceKHR(instance, _surface);

        setupDebugMessenger();


        // Select the PhysicalDevice to use
        // Query physical devices:
        auto physicalDevices = instance.enumeratePhysicalDevices();
        if (physicalDevices.empty()) {
            throw std::runtime_error("Failed to find GPUs with Vulkan Support!");
        }

        // Check if physcial devices found meet requirements
        for (auto pDevice : physicalDevices) {
            // Get device properties:
            auto deviceProperties = pDevice.getProperties();

            // Query feature support (Like multiple viewports, texture compression etc)
            auto deviceFeatures = pDevice.getFeatures();

            // Check if physicalDevice at least supports Vulkan 1.3
            bool supportsVulkan1_3 = pDevice.getProperties().apiVersion >= vk::ApiVersion13;

            // CHeck which queue families are supported and the commands we wish to use
            auto queueFamilies = pDevice.getQueueFamilyProperties();
            bool supportsGraphics = std::any_of(queueFamilies.begin(), queueFamilies.end(),
                [](auto const& qfp){return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

            // Check extensions are supported by device
            std::vector<const char*> requiredDeviceExtension = {vk::KHRSwapchainExtensionName};

            auto availableDeviceExtensions = pDevice.enumerateDeviceExtensionProperties();
            bool supportsAllRequiredExtensions =
                std::all_of(requiredDeviceExtension.begin(), requiredDeviceExtension.end(),
                    [&availableDeviceExtensions](auto const& requiredDeviceExtension)
                    {
                        return std::any_of(availableDeviceExtensions.begin(), availableDeviceExtensions.end(),
                            [requiredDeviceExtension](auto const& availableDeviceExtension)
                            {
                                return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0;
                            });
                    });

            // Check that all optionally supported required features are supported by VK
            auto features = pDevice.template getFeatures2<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
            bool supportsRequiredFeatures = features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

            std::cout << "Found compatible physical device: " << deviceProperties.deviceName << std::endl;
        }

        // Select first physical device. NOTE: USe  a scoring system off of layer & feature comaptabiolity/support to rank GPUs(in the case of mulltiple HW, or provide a Selection GUI)
        if (physicalDevices.size() > 0) {
            physicalDevice = physicalDevices[0];
            std::cout << "Selected physical device(GPU): " << physicalDevice.getProperties().deviceName << std::endl;
        } else {
            throw std::runtime_error("Failed to find a physical device!");
        }


        // Logical device Creation:
        //(Similar to creaitng a vkInstance, we provide info through a struct, then provide that info to the caller)

        // Specifies the queues to be created
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();
        auto graphicsQueueFamilyProperty = std::find_if(queueFamilyProperties.begin(), queueFamilyProperties.end(),
            [](auto const &qfp){ return (qfp.queueFlags & vk::QueueFlagBits::eGraphics) != static_cast<vk::QueueFlags>(0); });

        // Gaurd against index selection bug (If no grpahics queue exists, std::distance becomes std::distance9queueFamilyProeprties.size() -> invlaid synatx)
        if (graphicsQueueFamilyProperty == queueFamilyProperties.end()) {
            throw std::runtime_error("No graphics queue found!");
        }
        auto graphicsIndex = static_cast<uint32_t>(std::distance(queueFamilyProperties.begin(), graphicsQueueFamilyProperty));

        // VK requires priorties to be assigned(Seriously cool!) from 0.f to 1.f
        float queuePriority = 0.5f;

        vk::DeviceQueueCreateInfo deviceQueueCreateInfo{
            {},
            graphicsIndex,
            1,
            &queuePriority
        };

        // VK Structure chaining is where multiple device features can be linked via pNext param,
        // However, VK::Raii provides a StructureChain struct that essentially take care of this for us

        // Create a chain of feature structures
        vk::StructureChain<
            vk::PhysicalDeviceFeatures2,
            vk::PhysicalDeviceVulkan13Features,
            vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
        featureChain{}; // designated initalization used in tutorial(CXX20), use CXX17 more explict apprach by creating blank chain and accesing its members to modify

        // Modify the feature chain
        // vk::PhysicalDeviceFeatures2 (empty for now, hence not mentioned below)

        // Enable dynamic rendering from Vulkan 1.3 (Enable Vulkan 1.3 features)
        featureChain.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering = true;
        // Enable extended dynamic state from the extension 9enable extension features)
        featureChain.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState = true;

        // Specify device extensions:
        std::vector<const char*> requiredDeviceExtension = {
            vk::KHRSwapchainExtensionName // Required for rpesenting rendered images to the window
        };

        // Query for a supported graphics queue that supports both graphics and present
        /*uint32_t queueIndex = ~0;
        for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size(); qfpIndex++) {
            if ((queueFamilyProperties[qfpIndex].queueFlags & vk::QueueFlagBits::eGraphics) &&
                physicalDevice.getSurfaceSupportKHR(qfpIndex, *surface)) {

                // Found a queue family that support both graphics and present
                queueIndex = qfpIndex;
                break;
                }
        } if (queueIndex == ~0) {
            throw std::runtime_error("Could not find a queue for graphics and present -> termianting");
        }*/

        // Acctually create the Logical device now: (Info first)
        // First, find the index of the first queue family that sypports graphics
        //std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

        // Get the first index into queuefamilyProperties which supports both graphics adn present
        uint32_t queueIndex = ~0;
        for (uint32_t qfpIndex = 0; qfpIndex < queueFamilyProperties.size();)
        vk::DeviceCreateInfo deviceCreateInfo{
            {},
            1,
            &deviceQueueCreateInfo,
            0,
            nullptr, // no validaiton layers
            static_cast<uint32_t>(requiredDeviceExtension.size()),
            requiredDeviceExtension.data(), // These extensions are device specific, not isntance specific
            nullptr, // legacy features NOT used
            &featureChain.get<vk::PhysicalDeviceFeatures2>() // pNext chain (Starts from the firts iterator, VK poicks up the rest)
        };

        // WARNING: Can throw errors, if not disabled
        device = vk::raii::Device(physicalDevice, deviceCreateInfo);

        graphicsQueue = vk::raii::Queue(device, graphicsIndex, 0); // Retrieve a queue handle for one queue family. Only creatinging a single queue from this family -> use queueIndex(0)


        std::cout << "Layer ahs successfully initalised!\n";
    }

    void onUpdate() override {
        //std::cout << "Count: " << count << std::endl;
        //count++;

        if (count == 200) {
            AppState.isRunning = false;
        }

        if (glfwGetKey(engineAPI->getWindowCtx()->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            AppState.isRunning = false;
        }
    }

    void onRender() override {}
    void onShutdown() override {}
};

int main() {

    //Dexium::AppEngine engine;
    engineAPI = std::make_unique<Dexium::AppEngine>();

    engineAPI->attachWindow(std::make_unique<Dexium::Backends::WindowContext>(
        800, 600, "Dexium-Vulkan"));

    engineAPI->presentLayer("Game", std::make_unique<Game>());

    engineAPI->run();

    return 0;
}