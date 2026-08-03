//
// Created by ethan on 29/7/26.
//

// Defines a VkInstance structure for Dexium (Backend=Vulkan)

#ifndef DEXIUM_INSTANCE_HPP
#define DEXIUM_INSTANCE_HPP

// Engine related targets:
#include <utils/VersionControl.hpp>

namespace Dexium::backend {

    class Instance {
    public:
        Instance(const char* appName, const VersionControl& appVersion, const VersionControl& vkAPIVersion = {0,1,4,0});
        ~Instance();

        // Prvent copying, as its a RAII class that manages the underlying calls of VkInstance(vkDestroyInstance() called on dtor!!)
        Instance(const Instance&) = delete;
        Instance* operator=(const Instance&) = delete;

        VkInstance getInstance() const; // Internally VkInstance is jsut a ptr handle, so should be cheap to return by value

        const VkApplicationInfo& getAppInfo() const;

    private:
        // The isntance handle:
        VkInstance m_instance = VK_NULL_HANDLE; // Prevents weird bugs from inproper init

        // Stores the required app info to construct the instance
        VkApplicationInfo m_appInfo{};

        // The instance createInfo struct:
        VkInstanceCreateInfo m_createInfo{};
    };
}



#endif //DEXIUM_INSTANCE_HPP