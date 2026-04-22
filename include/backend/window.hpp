//
// Created by ethan on 18/4/26.
//

#ifndef DEXIUM_WINDOW_HPP
#define DEXIUM_WINDOW_HPP

#include <string_view>

#include <utils/BitwiseFlag.hpp>
#include <backend/Initalisers.hpp>

#include <GLFW/glfw3.h>

namespace Dexium::Backends {

    void errorCallback(int error, const char* description);

    enum class DxWindowFlags {
        DX_WINDOW_NONE = 1 << 0,
        DX_WINDOW_RESIZABLE = 1 << 1,
        DX_WINDOW_FULLSCREEN = 1 << 2,
        DX_WINDOW_MAXIMIZED = 1 << 3,
        DX_WINDOW_MINIMIZED = 1 << 4,
        DX_WINDOW_MINIMIZABLE = 1 << 5,
        DX_WINDOWPOS_CENTERED = 1 << 6
    };

    class WindowContext {
    public:
        WindowContext(int windowWidth, int windowHeight, std::string_view windowName, DxWindowFlags winFlags = DxWindowFlags::DX_WINDOW_NONE);
        ~WindowContext();

        void resizeWindow(int windowWidth, int windowHeight);

        GLFWwindow* window;

    private:
        int width, height;
        GLFWmonitor* m_monitor = nullptr;

        GLFWInitialiser glfwDevice;
    };
}


template<>
struct Dexium::Traits::EnableBitmaskOperators<Dexium::Backends::DxWindowFlags> {
    static constexpr bool value = true;
};

#endif //DEXIUM_WINDOW_HPP