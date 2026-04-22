//
// Created by ethan on 18/4/26.
//

#include <backend/Initalisers.hpp>

#include <iostream>

#include <GLFW/glfw3.h>

namespace Dexium::Backends {
    GLFWInitialiser::GLFWInitialiser() {
        Load();
    }

    bool GLFWInitialiser::isLoaded() {
        return m_isLoaded;
    }

    int GLFWInitialiser::Load() {
        if (!m_isLoaded) {
            if (!glfwInit()) {
                std::cout << "Failed to initialise GLFW\n";
                return -1;
            }
            m_isLoaded = true;
        }

        return 0;
    }

    void GLFWInitialiser::Unload() {
        if (m_isLoaded) {
            glfwTerminate();
        }
    }

    GLFWInitialiser::~GLFWInitialiser() {
        Unload();
    }
}
