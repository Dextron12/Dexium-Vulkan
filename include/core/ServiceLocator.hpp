//
// Created by ethan on 5/9/26.
//

#ifndef DEXIUM_SERVICELOCATOR_HPP
#define DEXIUM_SERVICELOCATOR_HPP

#include <memory>
#include <unordered_map>
#include <typeindex>

#include <fmt/base.h>
#include <fmt/color.h>

// Global engine instance defined!! <Dexium::Core::serviceLocator>
/* HOW TO SUE
 * Either use the serviceLocator(global var) directly, or create a local ref:
 * // auto& services = Dexium::Core::serviceLocator;
 * Then register a service, if a type isnt registered then get() is called, it will throw a runtime exception!
 * // services.add<T>();
 * Alternatively, if a service already exists, it can be moved into the serviceLocator:
 * // services.add<T>(std::move(*T));
 *
 * KEY:
 *      // -> Code
*/


namespace Dexium::Core {

    class ServiceRegistry {
        struct HolderBase {
            virtual ~HolderBase() = default;
        };

        template<typename T>
        struct Holder : HolderBase {
            template<typename... Args>
            Holder(Args&&... args) : value(std::forward<Args>(args)...) {}

            T value;
        };

    public:
        template<typename T, typename... Args>
        T& add(Args&&... args) {
            auto holder = std::make_unique<Holder<T>>(std::forward<Args>(args)...);

            T& result = holder->value;

            m_services[typeid(T)] = std::move(holder);
            return result;
        }

        template<typename T>
        T& add(std::unique_ptr<T> object) {
            if (!object) return nullptr; // Silent fail

            auto holder = std::make_unique<Holder<T>>(std::move(*object));

            T& result = holder->value;

            m_services[typeid(T)] = std::move(holder);
            return result;
        }

        template<typename T>
        T& get() {
            try {
                return static_cast<Holder<T>&>(*m_services.at(typeid(T))).value;
            } catch (const std::out_of_range& e) {
                fmt::print(stderr, fg(fmt::color::red), "FATAL RUNTIME ERROR: Uncaught Exception ->ServiceRegistry::get() called with an unregistered type\n {}\n", e.what());
                std::exit(1);
            }
        }

    private:
        std::unordered_map<std::type_index, std::unique_ptr<HolderBase>> m_services;
    };

    extern ServiceRegistry serviceLocator;
}

#endif //DEXIUM_SERVICELOCATOR_HPP
