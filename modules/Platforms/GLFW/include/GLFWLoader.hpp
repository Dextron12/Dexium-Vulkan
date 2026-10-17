//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_GLFWLOADER_HPP
#define DEXIUM_GLFWLOADER_HPP

#if defined(DX_RM_VULKAN)
#define GLFW_INCLUDE_VULKAN
#endif

#include <core/ILoader.hpp>

#include <GLFW/glfw3.h>

class GLFWLoader : public ILoader {
public:
    GLFWLoader() = default;

protected:
    bool onLoad() override {
        if (glfwInit() != GLFW_TRUE) return false;

        return true;
    }

    bool  onUnload() override {
        glfwTerminate();
        return true;
    }
};

#endif //DEXIUM_GLFWLOADER_HPP