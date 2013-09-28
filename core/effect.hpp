#ifndef CORE_EFFECT_HPP
#define CORE_EFFECT_HPP

#include <type_traits>
#include "defs.hpp"

struct GameObject;

TPL(T) struct Effect {
    static_assert(std::is_base_of<GameObject, T>::value,
        "T <: GameObject required in Effect<T>.");
    virt init(const T&)          -> void  {};
    virt free(Pool&)             -> void  {};
    virt update(const T&, float) -> void = 0;
    virt is_expired()     const  -> bool = 0;
    virt apply(T&, float) const  -> void = 0;
};

TPL(E) class CheckEffect {
    static_assert(sizeof(E) <= 32,
        "Effect subclasses should have sizeof <= 32 to fit into object pulls.");
};

#define CHECK_EFFECT(NAME, TYPE) struct NAME ## Checker : public CheckEffect<TYPE> {}

#endif CORE_EFFECT_HPP