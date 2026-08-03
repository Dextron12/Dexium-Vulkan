//
// Created by ethan on 29/7/26.
//

#ifndef DEXIUM_VKWINDOW_HPP
#define DEXIUM_VKWINDOW_HPP

#include <GLFW/glfw3.h>

#include <core/Loader.hpp>
#include <core/WindowContext.hpp>

namespace Dexium::backend {

    class glfwLoader : public Core::Loader {
    protected:
        void onLoad() override;
        void onUnload() override;
    };

    class VkWindow: Core::IWindowLoader {
    public:
        VkWindow() = default;

        bool Load(const char* windowName, int windowWidth, int windowHeight, glfwLoader* winLoader, std::initializer_list<Core::WindowFlag> flags) override;
        bool Unload() override;

        GLFWwindow* getWindow() const;

    protected:
        GLFWwindow* window = nullptr;
    };
}

#endif //DEXIUM_VKWINDOW_HPP