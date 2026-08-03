//
// Created by ethan on 29/7/26.
//

#include <backend/VkWindow.hpp>

#include <core/Error.hpp>

namespace Dexium::backend {

    void glfwLoader::onLoad() {
        if (!glfwInit()) {
            const char* errDesc = nullptr;
            int errCode = glfwGetError(&errDesc);
            TraceLog(Core::LogLevel::FATAL, "Failed to instantiate a GLFW instance!\nGLFW Error, code({}): {}", errCode, errDesc);
        }
    }

    void glfwLoader::onUnload() {
        glfwTerminate(); // These virt fns are guarded by the internal base m_loaded member & state is valdiated before executing these virt fns!
    }


    bool VkWindow::Load(const char* windowName, int windowWidth, int windowHeight, glfwLoader* winLoader, std::initializer_list<Core::WindowFlag> flags) {
        // Check that the windowing lib(GLFW for Vulkan backend) is instantiated
        if (!winLoader) {
            TraceLog(Core::LogLevel::FATAL, "No window loader provided to WindowContext(Vk)\nCannot create a window object!");
            return false; // TraceLog(Fatal) should make a call to std::exit, but return used for the exceptional failsafe
        }

        if (!winLoader->loaded()) {
            // Could try to lazy-init the loader, but we can't guarantee that another one doesn't exist and would also make it harder to debug in test env
            TraceLog(Core::LogLevel::FATAL, "Windowing loader reports status: unloaded!\nCannot create a window context");
            return false;
        }

        // Set a window hint so now OpenGl client API is configured for the window
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // For now, force a windowed frame with no resizing, as VK resizing requires a bit of extra work
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        // Set window(design) size:
        m_width = windowWidth;
        m_height = windowHeight;

        // Create window object
        window = glfwCreateWindow(m_width, m_height, windowName, nullptr, nullptr);
        if (!window) {
            const char* errorDesc = nullptr;
            int errCode = glfwGetError(&errorDesc);

            TraceLog(Core::LogLevel::FATAL, "Failed to create a window surface\nGLFW Error({}), {}", errCode, errorDesc);
            return false;
        }

        return true;

    }

    bool VkWindow::Unload() {
        if (m_loaded) {
            glfwTerminate();
            m_loaded = false;

            return true;
        }

        return false;
    }

    GLFWwindow* VkWindow::getWindow() const {
        return window;
    }
}
