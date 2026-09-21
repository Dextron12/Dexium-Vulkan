//
// Created by ethan on 12/9/26.
//

#ifndef DEXIUM_STRUCTURECHAIN_HPP
#define DEXIUM_STRUCTURECHAIN_HPP
#include <type_traits>
#include <bits/locale_facets_nonio.h>
#include <fmt/base.h>


// This is a custom implementation of the Vulkan CPP StructureChain feature
// Made custom to eventually integerate into custom metadata

namespace Dexium::Vulkan {

    template<typename... Ts>
    class StructureChain;

    // Singe-case specialisation
    template<typename T>
    class StructureChain<T> {
    public:
        StructureChain() {
            relink();
        }

        T* head() noexcept {
            return &value;
        }

        const T* head() const noexcept {
            return &value;
        }

        template<typename U>
        U* get() noexcept {
            static_assert(std::is_same_v<T, U>, "Requested type is not present in StructureChain");
            return &value;
        }

        template<typename U>
        const U* get() const {
            static_assert(std::is_same_v<T, U>, "Requested type is not present in StructureChain");
            return &value;
        }

    private:
        T value{};
    };


    // Multi-case specialisation
    template<typename T, typename U, typename... Rest>
    class StructureChain<T, U, Rest...> {
    public:
        StructureChain() {
            relink();
        }

        // Copy ctor
        StructureChain(const StructureChain& other) : value(other.value), next(other.next) {
            relink();
        }

        // Copya ssignment
        StructureChain& operator=(const StructureChain& other) {
            if (this != &other) {
                value = other.value;
                next = other.next;
                relink();
            }
            return *this;
        }

        // Move ctor
        StructureChain(StructureChain&& other) noexcept : value(std::move(other.value)), next(std::move(other.next)) {
            relink();
        }

        // Move assignment
        StructureChain& operator=(StructureChain&& other) noexcept {
            if (this != &other) {
                value = std::move(other.value);
                next = std::move(other.next);
                relink();
            }
            return *this;
        }

        T* head() {
            return &value;
        }

        const T* head() const {
            return &value;
        }

        template<typename V>
        V* get() noexcept {
            if constexpr (std::is_same_v<T, V>) {
                return &value;
            } else {
                return next.template get<V>();
            }
        }

        template<typename V>
        const V* get() const {
            if constexpr (std::is_same_v<T, V>){
                return &value;
            } else {
                return next.template get<V>();
            }
        }

        void relink() noexcept {
            //value.sType = VulkanStructureTraits<T>::sType; // Prevents sTypes changing or propagating through chain when chain is copied/moved
            value.pNext = next.head();
            next.relink();
        }

    private:
        T value{};
        StructureChain<U, Rest...> next;
    };
}

#endif //DEXIUM_STRUCTURECHAIN_HPP
