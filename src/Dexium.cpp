//
// Created by ethan on 18/4/26.
//

#include "Dexium.hpp"

#include <string>
#include <iostream>

namespace Dexium {
    AppEngine::AppEngine() {

    }

    int AppEngine::presentLayer(std::string_view layerName, std::unique_ptr<Core::ApplicationLayer> appLayer) {
        std::string pName(layerName);

        // Check if a layer of same name already exists
        if (m_engineLayers.find(pName) != m_engineLayers.end()) {
            return -1;
        }

        // Check if layer is inited (init if not)
        if (!appLayer->AppState.isPaused && !appLayer->AppState.isRunning) {
            // Layer is shutdown, init it
            appLayer->onInit();
            // Now flag that the layer is running
            appLayer->AppState.isRunning = true;
        }

        // If Layer is paused, resume the layer
        if (appLayer->AppState.isPaused) {
            appLayer->AppState.isRunning = false; // Unpause the layer
        }

        // Move the layerObj into the engine
        auto [it, inserted] = m_engineLayers.emplace(pName, std::move(appLayer));

        return it->second->errState.code;
    }

    int AppEngine::pauseLayer(std::string_view layerName) {
        std::string pName(layerName);

        // Find the layer in the map
        auto it = m_engineLayers.find(pName);
        if (it != m_engineLayers.end()) {
            it->second->AppState.isPaused = true;
            return 0;
        }

        // Layer not found, return errCode(-1)
        return -1;
    }

    int AppEngine::stopLayer(std::string_view layerName) {
        std::string pName(layerName);

        auto it = m_engineLayers.find(pName);
        if (it != m_engineLayers.end()) {
            it->second->AppState.isRunning = false;
            it->second->AppState.isPaused = false;
            // Call the Layer::onShutdown()
            it->second->onShutdown();

            // Layer now shutdown (and can be restarted with presentLayer(name, nullptr))

            // Return the layerCode
            return it->second->errState.code;
        }

        return -1;
    }

    void AppEngine::unloadLayer(std::string_view layerName) {
        std::string pName(layerName);

        // Check if layer exists (then pop it from map)
        auto it = m_engineLayers.find(pName);
        if (it != m_engineLayers.end()) {
            m_engineLayers.erase(it);
        }
    }

    void AppEngine::run() {
        while (m_appState) {
            //Itertate through the layers and execute them

            int activeLayerCount = 0; // Increments for every active layer
            for (const auto& [layerName, ptr] : m_engineLayers) {
                if (ptr->AppState.isRunning) {
                    ptr->onUpdate();
                    ptr->onRender();

                    activeLayerCount++;
                }
            }

            // If No layers exist, or activeLayerCount = 0, exit application
            if (m_engineLayers.empty() || activeLayerCount == 0) {
                // shutdown engine loop
                std::cout << "No Active layers to execute within the Engine. Shutting down!" << std::endl;
                m_appState = false;
                shutdown(); // Engine::shutdown being called (Not layer)
            }
        }
    }

    void AppEngine::shutdown() {
        // Iterate through the running layers and shut them down
        for (const auto& [layerName, ptr] : m_engineLayers) {
            const auto& appState = ptr->AppState;

            if (appState.isRunning || appState.isPaused) {
                ptr->onShutdown();
            }
        }
    }
}
