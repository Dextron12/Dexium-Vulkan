//
// Created by ethan on 3/9/26.
//

#ifndef DEXIUM_ILOADER_HPP
#define DEXIUM_ILOADER_HPP

#include <tuple>

class ILoader {
public:
    virtual ~ILoader() = default;

    // Delete copy ctor
    ILoader(const ILoader&) = delete;
    // Delete move ctor (Deleteing copy operations implicitly delets move operations)
    ILoader(ILoader&&) = delete;

    // Delete copy assignment operator
    ILoader& operator=(const ILoader&) = delete;
    // Delete move operator
    ILoader& operator=(ILoader&&) = delete;

    void load() {
        if (m_loaded) return;

        if (onLoad() && !m_loaded) m_loaded = true;
    }

    void unload() {
        if (!m_loaded) return;

        if (onUnload() && m_loaded) m_loaded = false;
    }

    bool isLoaded() const {
        return m_loaded;
    }

protected:
    ILoader() = default;

    virtual bool onLoad() = 0;
    virtual bool onUnload() = 0;

    bool m_loaded = false;


};

#endif //DEXIUM_ILOADER_HPP