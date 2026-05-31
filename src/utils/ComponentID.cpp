//
// Created by ethan on 31/5/26.
//

#include <utils/ComponentID.hpp>


namespace Dexium::Utils {
    uint32_t CompID::generateID() {
        auto& instance = get();
        uint32_t id;

        // Check for a free ID (Thread locked)
        std::lock_guard<std::mutex> lock(instance.mtx);
        if (!instance.freeIDs.empty()) {
            id = instance.freeIDs.front(); instance.freeIDs.pop();
            instance.freeSet.erase(id);
        } else {
            // Increment generator
            instance.generator++;
            id = instance.generator;
        }

        return id;
    }

    void CompID::destroyID(const uint32_t& ID) {
        auto& instance = get();

        std::lock_guard<std::mutex> lock(instance.mtx);

        // Check that value is within range of the generator
        if (ID == 0 || ID > instance.generator) return; // ID is invalid, nothing to free
        if (!instance.freeSet.insert(ID).second) return; // Already freed
        // Cannot reasonably check if ID already is in the queue, without using deque or a copy
        // This check is trivial anyway if the system is proeprly used

        instance.freeIDs.push(ID);
    }
}

/*
namespace Dexium::Utils {
    uint32_t CompID::generateID() {
        auto& instance = get();
        uint32_t id;

        // Check for a vacant ID
        if (!instance.freeIDs.empty()) {
            // Recyle ID
            id = instance.usedIDs[0]; // Grab the first ID
            // Erase the ID from the usedVector
            instance.usedIDs.erase(instance.usedIDs.begin());
        } else {
            // Increment the generator and use new ID:
            instance.generator++;
            id = instance.generator;
        }

        return id;
    }

    void CompID::destroyID(const uint32_t& ID) {
        auto& instance = get();

        // Check if that ID is NOT in use:
        bool found = false;
        for (const auto& i : instance.usedIDs) {
            if (i == ID) found = true;
        }
        // A simple search pattern, not sure if its fast!

        if (!found) {
            instance.usedIDs.emplace_back(std::move(ID));
        }
    }
}
*/
