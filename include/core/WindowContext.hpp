//
// Created by ethan on 3/8/26.
//

#ifndef DEXIUM_WINDOWCONTEXT_HPP
#define DEXIUM_WINDOWCONTEXT_HPP

#include <initializer_list>

namespace Dexium::Core {

    struct WindowFlag {
        int hint = 0;
        int value = 0;

        WindowFlag(int hint, int value) : hint(hint), value(value) {}
        WindowFlag(int hint) : hint(hint), value(1) {}
    };

    class IWindowLoader {
    public:
        IWindowLoader() = default;
        virtual ~IWindowLoader();

        virtual bool Load(const char* windowName, int windowWidth, int windowHeight, std::initializer_list<WindowFlag> flags = {}) = 0;
        virtual bool Unload() = 0;

        int getWidth() const {
            return m_width;
        }

        int getHeight() const {
            return m_height;
        }

        bool isLoaded() const {
            return m_loaded;
        }

    protected:
        bool m_loaded = false;

        int m_width = 0;
        int m_height = 0;
    };
}

#endif //DEXIUM_WINDOWCONTEXT_HPP