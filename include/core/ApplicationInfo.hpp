//
// Created by ethan on 9/9/26.
//

#ifndef DEXIUM_APPLICATIONINFO_HPP
#define DEXIUM_APPLICATIONINFO_HPP


#include <cstdint>
#include <math.h>
#include <string>

#include <tuple>



namespace Dexium::Core {

    struct WindowRatio {
        int width = 0;
        int height = 0;

        //std::tuple<double, double> getRatio() {
            // Using Eculidean Algorith to find GCD

            /*float gcd = 0;
            float l = 0;
            float s = 0;

            if (width > height) {
                l = static_cast<float>(width); s = static_cast<float>(height);
            } else {
                l = static_cast<float>(height); s = static_cast<float>(width);
            }

            while (true) {
                auto res = std::fmod(l, s);

                // CVheck absolute closeness (Values near 0)
                if (std::abs(l - s) <= 1e-5f) {
                    if ()
                }
            }*/
        //}
    };
}


#if defined(DX_RM_VULKAN)

#include <vulkan/vulkan.hpp>

namespace Dexium::Vulkan {

    class ApplicationInfo {
    public:
        ApplicationInfo(const std::string& windowTitle, Core::WindowRatio windowSize, uint32_t appVersion) {
            appInfo = VkApplicationInfo{
                .pApplicationName = windowTitle.c_str(),
                .applicationVersion = appVersion,
                .pEngineName = "Dexium-Framework",
                .engineVersion = VK_MAKE_VERSION(0,20,0),
                .apiVersion = VK_MAKE_API_VERSION(0,1,4,0)      // API version isn't garuanted, device may support a lower version
            };
        }

        VkApplicationInfo appInfo;



    };
}

#endif

#endif //DEXIUM_APPLICATIONINFO_HPP
