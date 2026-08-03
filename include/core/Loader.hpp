//
// Created by ethan on 3/8/26.
//

#ifndef DEXIUM_LOADER_HPP
#define DEXIUM_LOADER_HPP


namespace Dexium::Core {

    class Loader {
    public:
        Loader() = default;

        virtual ~Loader() {
            if (m_loaded) {
                onUnload();
            }
        }

        void load() {
            if (m_loaded) return;

            onLoad();
            m_loaded = true;
        }

        void unload() {
            if (!m_loaded) return;

            onUnload();
            m_loaded = false;
        }

        bool loaded() const {
            return m_loaded;
        }

    protected:
        // Instead of using pure virtual fns, this allows for argument overloading on the base fns
        virtual void onLoad(){};
        virtual void onUnload(){};

    private:
        bool m_loaded = false;
    };
}

#endif //DEXIUM_LOADER_HPP