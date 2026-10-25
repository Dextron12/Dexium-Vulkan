#ifdef DX_PLATFORM_GLFW
#include <Platforms/GLFW/include/GLFWLoader.hpp>
#include <Platforms/GLFW/include/GLFWindow.hpp>
#endif

#ifdef DX_RM_VULKAN
#include <RenderBackends/Vulkan/include/Instance.hpp>
#include <RenderBackends/Vulkan/include/Device.hpp>
#endif

#include <core/ApplicationInfo.hpp>
#include <core/VersionControl.hpp>

#include <util/Logger.hpp>

#include <fmt/base.h>
#include <fmt/format.h>


int main() {

    auto& services = Dexium::Core::serviceLocator;
    services.add<Logger>();
    services.get<Logger>().connectSink((std::make_unique<StderrSink>()));

    GLFWLoader glfwLoader;
    glfwLoader.load();

    if (!glfwLoader.isLoaded()) {
        const char* err = nullptr;
        glfwGetError(&err);
        TraceLog(LogLevel::FATAL, "GLFW failed to initialise!\nStatus: {}", err);
        return EXIT_FAILURE;
    }

    Dexium::Platform::GLFW::Window window(glfwLoader, "Dx Test Project", 1080, 720);

    Dexium::Vulkan::ApplicationInfo appInfo(window, Dexium::Core::VersionControl{0,0,0}.makeVersion());

    Dexium::Vulkan::Instance instance(appInfo);

    const auto& res = instance.enableExtensions(Dexium::Platform::GLFW::QueryVulkanInstanceExtensions());
    if (res.noFailedEXT > 0) {
        for (const auto& ext : res.unsupportedEXT) {
            TraceLog(LogLevel::ERROR, "No support for extension '{}'", ext);
        }
    }

    // Setup the VK validation layer & debugger
    const auto& uLayers = instance.enableValidationLayers({"VK_LAYER_KHRONOS_validation"});
    if (!uLayers.empty()) {
        for (const auto& layer : uLayers) {
            TraceLog(LogLevel::FATAL, "Vk driver does not support the requested validation layer: '{}'", layer);
        }
        return EXIT_FAILURE;
    }

    // Create instance
    if (!instance.createInstance()) {
        TraceLog(LogLevel::FATAL, "Failed to create a VkInstance. Aborting...)");
        return EXIT_FAILURE;
    }

    // Set up a VkSurface for the window
    auto surface = instance.createWindowSurface(window.getWindow());
    if (surface == nullptr) {
        TraceLog(LogLevel::FATAL, "Failed to create a window surface!");
        return EXIT_FAILURE;
    }

    const auto& deviceList = instance.enumeratePhysicalDevices();

    // Skip device selection criteria, if ONLY one device found, must still pass engine min requirements
    auto pDev = Dexium::Vulkan::physicalDevice(nullptr);
    if (deviceList.size() > 1) {
        TraceLog(LogLevel::WARN, "No device selection process implemented yet. Cannot automatically deduce device selection");
        return EXIT_FAILURE;
    } else if (deviceList.size() == 1) {
        pDev = Dexium::Vulkan::physicalDevice(deviceList[0]);
    }

    auto qf = pDev.getDefaultQueueFamily(surface);

    fmt::print("");



}
