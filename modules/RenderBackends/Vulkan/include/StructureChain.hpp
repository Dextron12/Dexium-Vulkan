//
// Created by ethan on 12/9/26.
//

#ifndef DEXIUM_STRUCTURECHAIN_HPP
#define DEXIUM_STRUCTURECHAIN_HPP

#include <type_traits>


// This is a custom implementation of the Vulkan CPP StructureChain feature
// Made custom to eventually integerate into custom metadata

#include <RenderBackends/Vulkan/include/VkStructureTraits.hpp> // Vulkan structure metadata: Generated from VkReflect (git@github.com:Dextron12/VkReflect)

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

        void relink() noexcept {
            value.pNext = nullptr;
            value.sType = VkStructureTraits<T>::sType;
        }

        template<typename Fn>
        void compareStructure(const StructureChain& other, Fn&& fn) const {
            forEachMemberPair(value, other.value, fn);
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
            value.sType = VkStructureTraits<T>::sType; // Prevents user-modified sType properagting through a copy
            value.pNext = next.head();
            next.relink();
        }

        template<typename Fn>
        void compareStructure(const StructureChain& other, Fn&& fn) const {
            forEachMemberPair(value, other.value, fn);
            next.compareStructure(other.next, fn);
        }

    private:
        T value{};
        StructureChain<U, Rest...> next;
    };
}

#endif //DEXIUM_STRUCTURECHAIN_HPP
