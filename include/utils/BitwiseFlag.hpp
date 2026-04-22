//
// Created by ethan on 18/4/26.
//

#ifndef DEXIUM_BITWISEFLAG_HPP
#define DEXIUM_BITWISEFLAG_HPP

#include <type_traits>

// This header defines a standard btiwise flag enumeration systen that is sued across the project
//for various enum definitions. This header is only a template for the system


// Must be defined at top-level namespace to propigate downwards into other namespaces
namespace Dexium::Traits {

    template<typename E>
    struct EnableBitmaskOperators {
        static constexpr bool enableBitmaskOperators = true;
    };

    // define the operators

    // OR
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, E>
    operator|(E lhs, E rhs) {
        using U = typename std::underlying_type<E>::type;
        // convert the operation to the underlying type(U) then return the enum type (E)
        return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
    }

    // AND
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, E>
    operator&(E lhs, E rhs) {
        using U = typename std::underlying_type<E>::type;
        return static_cast<E>(
            static_cast<U>(lhs) & static_cast<U>(rhs)
        );
    }

    // NOT
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, E>
    operator~(E v) {
        using U = typename std::underlying_type<E>::type;
        return static_cast<E>(~static_cast<U>(v));
    }

    // OR assignment
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, E&>
    operator|=(E& lhs, E& rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    // AND assignment
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, E&>
    operator&=(E& lhs, E rhs) {
        lhs = lhs & rhs;
        return lhs;
    }

    // Helper fn for checking if a Bitwise enum class has a set flag
    // - the first param should be the enum class var, second should be the flag your checking against
    template<typename E>
    constexpr std::enable_if_t<EnableBitmaskOperators<E>::value, bool>
    hasFlag(E value, E flag) {
        return (value & flag) == flag;
    }

}

#endif //DEXIUM_BITWISEFLAG_HPP