//
// Created by ethan on 3/9/26.
//

#ifndef DX_PLATFORM_GLFW_Window_HPP
#define DX_PLATFORM_GLFW_Window_HPP


// Ln: 736(mains.cpp) // Load GLFW

#include <core/IWindow.hpp>
#include <core/ILoader.hpp>

#include <GLFW/glfw3.h>

#include <vector>

namespace Dexium::Platform::GLFW {

#ifdef DX_RM_VULKAN
    // Request GLFW required extensions
    std::vector<const char*> QueryVulkanInstanceExtensions();


#endif

    class Window : public IWindow {
    public:
        explicit Window(const ILoader& loader, const char* windowTitle, int windowWidth, int windowHeight);

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        ~Window() override;

        [[nodiscard]] GLFWwindow* getWindow() const;

        // Set a window size callback
        void setWindowSizeCallback(GLFWwindowsizefun func);

    private:
        GLFWwindow* m_window;

        bool framebufferResized = false;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    };
}

#endif //DX_PLATFORM_GLFW_Window_HPP