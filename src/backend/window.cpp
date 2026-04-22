//
// Created by ethan on 18/4/26.
//

// VK implementation of Dexium::WindowContext

#include <iostream>
#include <ostream>

#include <backend/window.hpp>
#include <backend/Initalisers.hpp>

#include <GLFW/glfw3.h>

#include <stdio.h>

Dexium::Backends::WindowContext::WindowContext(int windowWidth, int windowHeight, std::string_view windowName, DxWindowFlags winFlags) {
    width = windowWidth;
    height = windowHeight;

    // Validate GLFW initalisation
    if (!glfwDevice.isLoaded()) return;

    glfwSetErrorCallback(errorCallback);

    // Check fullscreen status
    if (Traits::hasFlag(winFlags, DxWindowFlags::DX_WINDOW_FULLSCREEN)) {
        // Create a fullscreen window on the primary monitor
        m_monitor = glfwGetPrimaryMonitor(); // use glfwGetMonitors() for a list of available monitors
    }

    // Instruct GLFW not to configure a GL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Configure window flags

    // (Resizing):
    if (Traits::hasFlag(winFlags, DxWindowFlags::DX_WINDOW_RESIZABLE)) {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    } else {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    // Create a window (With no GL context)
    window = glfwCreateWindow(width, height, windowName.data(), m_monitor, nullptr);
    if (!window) {
        std::cout << "Failed to create a window context" << std::endl;
        return;
    }

}

Dexium::Backends::WindowContext::~WindowContext() {
    //glfwDestroyWindow(window);

    // No need to call GLFWTerminate()! GLFWDevice is a RAII object that calls this fn when out of scope

}

void Dexium::Backends::WindowContext::resizeWindow(int windowWidth, int windowHeight) {
    width = windowWidth;
    height = windowHeight;
}


void Dexium::Backends::errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}
