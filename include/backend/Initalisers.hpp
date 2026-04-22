//
// Created by ethan on 18/4/26.
//

#ifndef DEXIUM_INITIALISERS_HPP
#define DEXIUM_INITIALISERS_HPP

namespace Dexium::Backends {

    class GLFWInitialiser {
    public:
        GLFWInitialiser();
        ~GLFWInitialiser();

        bool isLoaded();

        int Load(); // Returns 0 on success, -1 on failure
        void Unload();

    private:
        bool m_isLoaded = false;
    };
}

#endif //DEXIUM_INITIALISERS_HPP