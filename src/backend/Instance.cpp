//
// Created by ethan on 29/7/26.
//

#include <core/Error.hpp>
#include <backend/Instance.hpp>

namespace Dexium::backend {
    Instance::Instance(const char* appName, const VersionControl& appVersion, const VersionControl& vkAPIVersion) {
        // Configure appInfo:
        m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        m_appInfo.pApplicationName = appName;
        m_appInfo.applicationVersion = appVersion.makeVersion();

        // engine specific details:
        m_appInfo.pEngineName = "Dexium Framework";
        m_appInfo.engineVersion = ENGINE_VERSION.makeVersion();
        m_appInfo.apiVersion = vkAPIVersion.makeAPIVersion();

        // COnfigure Instance createInfo
        m_createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        m_createInfo.pApplicationInfo = &m_appInfo;
        m_createInfo.enabledLayerCount = 0;
        m_createInfo.ppEnabledLayerNames = nullptr;
        m_createInfo.enabledExtensionCount = 0;
        m_createInfo.ppEnabledExtensionNames = nullptr;

        // Attempt to create instance:
        if (vkCreateInstance(&m_createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            TraceLog(Core::LogLevel::FATAL, "[VkInstance]: Failed to create an instance!");
        }
    }

    Instance::~Instance() {
        if (m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
        }
    }

    VkInstance Instance::getInstance() const {
        return m_instance;
    }

    const VkApplicationInfo& Instance::getAppInfo() const {
        return m_appInfo;
    }
}
