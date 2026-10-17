//
// Created by ethan on 3/9/26.
//

#include <Platforms/GLFW/include/GLFWindow.hpp>

#include <util/Logger.hpp>

#if defined(DX_WM_GLFW)

//IWindow dtor definition
IWindow::~IWindow() = default;

namespace Dexium::Platform::GLFW {
    std::vector<const char*> QueryVulkanInstanceExtensions() {
        uint32_t extCount = 0;
        const char** ext = glfwGetRequiredInstanceExtensions(&extCount);

        return {ext, ext + extCount};
    }

    Window::Window(const ILoader& loader, const char* windowTitle, int windowWidth, int windowHeight)
        : IWindow(windowTitle, windowWidth, windowHeight) {

        //Ensure GLFW is loaded
        if (!loader.isLoaded()) {
            TraceLog(LogLevel::FATAL, "GLFW not loaded!");
            return; // Both IWindow & Window inited at this point, returning here makes the object useless as window creation code only occurs in the ctor after thsic heck
        }

        // Hint to GLFW to disable a GLFW context if Vulkan RM defined
#if defined(DX_RM_VULKAN)
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

        // Create window
        m_window = glfwCreateWindow(m_width, m_height, windowTitle, nullptr, nullptr);

        // Check window creation
        if (!m_window) {
            TraceLog(LogLevel::FATAL, "Failed to create a GLFW window!");
            return;
        }

        // Set GLFW user ptr to this
        glfwSetWindowUserPointer(m_window, this);

        // Set default framebuffer size callback (Must be a static fn)
        setWindowSizeCallback(framebufferResizeCallback);
    }


    GLFWwindow* Window::getWindow() const {
        return m_window;
    }

    void Window::setWindowSizeCallback(GLFWwindowsizefun func) {
        // Ensure window context is valid
        if (m_window) {
            glfwSetWindowSizeCallback(m_window, func);
        }
    }

    void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        self->m_width = width;
        self->m_height = height;

        self->framebufferResized = true;
    }

    Window::~Window()  {
        if (m_window) {
            glfwSetWindowSizeCallback(m_window, nullptr);
            glfwSetWindowUserPointer(m_window, nullptr);

            glfwDestroyWindow(m_window);
        }

    }
}

#endif