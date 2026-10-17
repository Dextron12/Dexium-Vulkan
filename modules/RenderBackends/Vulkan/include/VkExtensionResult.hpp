//
// Created by ethan on 17/10/26.
//

#ifndef DEXIUM_VKEXTENSIONRESULT_HPP
#define DEXIUM_VKEXTENSIONRESULT_HPP

#include <vector>

struct DX_EXT_PACK {
    size_t noEnabledEXT = 0; // The number of GLFW extensions that are required
    size_t noFailedEXT = 0; // Number of extensions that are NOT supported by the VK driver

    std::vector<const char*> unsupportedEXT; // List of GLFW EXT that the Vk driver doesn't support
    std::vector<const char*> enabledEXT; // List of GLFW extensions that SHOULD be added to instance::extensions to enable them
};

#endif //DEXIUM_VKEXTENSIONRESULT_HPP
