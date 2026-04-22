//
// Created by ethan on 18/4/26.
//

#ifndef DEXIUM_DEXIUM_HPP
#define DEXIUM_DEXIUM_HPP

#include <string_view>
#include <memory>
#include <unordered_map>

#include <core/Layer.hpp>


// Backend specifications
#ifdef DX_VK_BACKEND
#include <backend/window.hpp>
#endif

namespace Dexium {
    class AppEngine {
    public:
        AppEngine();

        // Pushes the Layer into engine and initalises it.
        int presentLayer(std::string_view layerName, std::unique_ptr<Core::ApplicationLayer> appLayer);
        int pauseLayer(std::string_view layerName);

        // Stops the layer from executing (But does not unload it)
        int stopLayer(std::string_view layerName);

        // Unload the Layer and all of its associated data
        void unloadLayer(std::string_view layerName);

        // Attach a new window context onto the engine
        void attachWindow(std::unique_ptr<Backends::WindowContext> windowPtr);

        Backends::WindowContext* const getWindowCtx() const;

        void run();

        void shutdown();

    private:
        std::unordered_map<std::string, std::unique_ptr<Core::ApplicationLayer>> m_engineLayers;

        bool m_appState = true;

        std::unique_ptr<Backends::WindowContext> m_Window = nullptr;
    };
}

#endif //DEXIUM_DEXIUM_HPP