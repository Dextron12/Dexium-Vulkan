#include <iostream>

#include <GLFW/glfw3.h>

#include <Dexium.hpp>

class Game : public Dexium::Core::ApplicationLayer {
public:

    int count = 0;

    void onInit() override {
        std::cout << "Hello, World!" << std::endl;
    }

    void onUpdate() override {
        std::cout << "Count: " << count << std::endl;
        count++;

        if (count == 200) {
            AppState.isRunning = false;
        }
    }

    void onRender() override {}
    void onShutdown() override {}
};

int main() {

    Dexium::AppEngine engine;

    engine.presentLayer("Game", std::make_unique<Game>());

    engine.run();

    return 0;
}