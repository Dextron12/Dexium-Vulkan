//
// Created by ethan on 18/4/26.
//

#ifndef DEXIUM_LAYER_HPP
#define DEXIUM_LAYER_HPP

#include <string>

namespace Dexium::Core {

    // Stores the error/exit code of the layer and a string for any debug messages
    class AppLayerErrState {
    public:
        std::string errMsg;
        int code = 0;
    };

    class AppLayerRunState {
    public:
        bool isRunning = false;
        bool isPaused = false;

        // if NOT isRunning & isPaused: Layer is inactive/shutdown
        // else, can assume layer has been initialised
    };


    class ApplicationLayer {
    public:

        AppLayerErrState errState;
        AppLayerRunState AppState;

        virtual void onInit() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onShutdown() = 0;

        virtual ~ApplicationLayer() = default;
    };


}

#endif //DEXIUM_LAYER_HPP