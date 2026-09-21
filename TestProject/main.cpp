
#include <fmt/base.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

#include <GLFWLoader.hpp>


#include <util/Logger.hpp>

#include <core/ServiceLocator.hpp>

#include "core/FileSystem.hpp"

#include <GLFWindow.hpp>

#include <Instance.hpp>

#include "core/VersionControl.hpp"

int main() {
    fmt::print(stderr, fg(fmt::color::crimson), "Hello World!\n");

    GLFWLoader glfwLoader;
    glfwLoader.load();

    Dexium::Core::Window window(glfwLoader, "Test Window", 1080, 720);

    if (!glfwLoader.isLoaded()) {
        fmt::print(stderr, fg(fmt::color::crimson), "Failed to load GLFW\n");
    }

    auto& services = Dexium::Core::serviceLocator;
    services.add<Logger>();

    services.add<Dexium::Core::Filesystem>();
    auto& vfs = services.get<Dexium::Core::Filesystem>();
    // Change path from out of build dir to main
    vfs.setWorkingDir(vfs.getWorkingDir().parent_path().parent_path());

    auto& logger = services.get<Logger>();
    logger.connectSink(std::make_unique<StderrSink>());

    TraceLog(LogLevel::DEBUG, "Hello, World!");


    TraceLog(LogLevel::INFO, "ExecPath: {}", vfs.getExecDir().c_str());
    TraceLog(LogLevel::INFO, "Working path: {}", vfs.getWorkingDir().c_str());

    Formatter mat; mat.showLocation = true;
    TraceLogFormat(LogLevel::WARN, mat, "Hello {} testing land!", "from");

    //Logger logger;
    //logger.connectSink(std::make_unique<StderrSink>());

    auto t = std::source_location::current();

    logger.write(LogLevel::INFO, std::source_location::current(), "This is an info log");
    logger.write(LogLevel::DEBUG, std::source_location::current(), "This is a debug log");
    logger.write(LogLevel::TRACE, std::source_location::current(), "This is a trace log");

    logger.write(LogLevel::WARN, mat, std::source_location::current(), "This is a warn log");

    logger.write(LogLevel::ERROR, std::source_location::current(), "This is an error log");
    logger.write(LogLevel::FATAL, std::source_location::current(), "This is fatal log");

    Dexium::Core::VersionControl ver{0,0,20};
    Dexium::Vulkan::ApplicationInfo appInfo{"Test Window", Dexium::Core::WindowRatio{.width = 1080,.height = 720}, ver.makeVersion()};
    //Dexium::Vulkan::Instance instance(appInfo);

    Dexium::Vulkan::Instance instance({"Test Window", Dexium::Core::WindowRatio{.width = 1080,.height = 720}, Dexium::Core::VersionControl{0,0,20}.makeVersion()});

    auto unsupportedExtensions = instance.checkGLFWExtensions();
    for (auto& extName : unsupportedExtensions) {
        TraceLog(LogLevel::WARN, "This Vulkan driver does not support the GLFW extension '{}'", extName);
    }

    auto unsupportedLayers = instance.enableValidationLayers({"VK_LAYER_KHRONOS_validation"});
    for (auto& layerName : unsupportedLayers) {
        TraceLog(LogLevel::WARN, "This Vulkan driver does not support the validation layer '{}'", layerName);
    }

    instance.createInstance();

    while (!glfwWindowShouldClose(window.getWindow())) {
        glfwPollEvents();

        if (glfwGetKey(window.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window.getWindow(), GL_TRUE);
        }
    }

}
