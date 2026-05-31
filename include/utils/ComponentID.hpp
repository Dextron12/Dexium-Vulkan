//
// Created by ethan on 31/5/26.
//

#ifndef DEXIUM_COMPONENTID_HPP
#define DEXIUM_COMPONENTID_HPP


#include <cstdint>
#include <mutex>

#include <queue>
#include <unordered_set>

namespace Dexium::Utils {

    class CompID {
    public:
        // Request a new ID from the generator
        [[nodiscard]] static uint32_t generateID();

        // Return the generated ID back to the generator
        static void destroyID(const uint32_t& ID);

        static CompID& get() {
            static CompID instance;
            return instance;
        }

    private:
        CompID() = default;

        uint32_t generator = 0;

        std::mutex mtx = std::mutex();

        std::queue<uint32_t> freeIDs = std::queue<uint32_t>();
        std::unordered_set<uint32_t> freeSet = std::unordered_set<uint32_t>();

    };
}

#endif //DEXIUM_COMPONENTID_HPP