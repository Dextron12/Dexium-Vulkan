//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_VULKAN2_IWINDOW_HPP
#define DEXIUM_VULKAN2_IWINDOW_HPP

#include <string>

class IWindow {
public:
    IWindow(const char* windowTitle, int windowWidth, int windowHeight) :
    m_windowTitle(windowTitle), m_width(windowWidth), m_height(windowHeight) {}

    virtual ~IWindow();

    int& getDesignWidth() {
        return m_width;
    }

    int& getDesignHeight() {
        return m_height;
    }

protected:
    std::string m_windowTitle;

    int m_width;
    int m_height;


};

#endif //DEXIUM_VULKAN2_IWINDOW_HPP