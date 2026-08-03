//
// Created by ethan on 29/7/26.
//

// Provides utility functions to bootstrap a GLFW + Vulkan backend

#ifndef DEXIUM_BOOTSTRAP_HPP
#define DEXIUM_BOOTSTRAP_HPP

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <core/Loader.hpp>

namespace Dexium::backend {


}


namespace Dexium::Core {

    enum class ENGINE_STATE {
        DEINITIALISED = 0,
        INITIALISED = 1,
        RUNNING = 2,
        PAUSED = 3,
        STOPPING = 4,
    };

    struct bootstrap_returnPacket {
        int err_code = 0; // 0 for success
        ENGINE_STATE state_code = ENGINE_STATE::DEINITIALISED;
    };

    class bootstrapLoader {
    public:
        bootstrapLoader();
        virtual ~bootstrapLoader() = default;

        virtual bootstrap_returnPacket initialise(const char* appName, int windowWidth, int windowHeight);
        virtual bootstrap_returnPacket shutdown();

    protected:
        ENGINE_STATE m_state;


    };
}

namespace Dexium::backend {



    class GLFWInitaliser {
    public:
        GLFWInitaliser();
        ~GLFWInitaliser();

        const bool isLoaded() const;

        // Prevent copies
        GLFWInitaliser(const GLFWInitaliser&) = delete;
        GLFWInitaliser& operator=(const GLFWInitaliser&) = delete;

        GLFWInitaliser(GLFWInitaliser&&) = delete;
        GLFWInitaliser& operator=(GLFWInitaliser&&) = delete;

    private:
        bool m_isLoaded = false;
    };
}

#endif //DEXIUM_BOOTSTRAP_HPP