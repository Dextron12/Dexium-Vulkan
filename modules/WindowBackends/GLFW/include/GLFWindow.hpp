//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_GLFWINDOW_HPP
#define DEXIUM_VULKAN2_GLFWINDOW_HPP


// Ln: 736(mains.cpp) // Load GLFW

#include <core/IWindow.hpp>
#include <core/ILoader.hpp>

#include <GLFW/glfw3.h>

#include <vector>

namespace Dexium::Core {
    class Window : IWindow {
    private:
        struct glfwExtensions {
            const char** extensions;
            uint32_t count;
        };
    public:
        //GLFWwindow(const ILoader& loader, const char* windowTitle, int windowWidth, int windowHeight)
        //    : IWindow(windowTitle, windowWidth, windowHeight) {}
        explicit Window(const ILoader& loader, const char* windowTitle, int windowWidth, int windowHeight);

        ~Window() override;

        [[nodiscard]] GLFWwindow* getWindow() const;

        // Set a window size callback
        void setWindowSizeCallback(GLFWwindowsizefun func);

        glfwExtensions getRequiredExtensions();

    private:
        GLFWwindow* m_window;

        bool framebufferResized = false;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    };
}

#endif //DEXIUM_VULKAN2_GLFWINDOW_HPP