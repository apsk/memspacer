#ifndef EFFECTS_ROTATION_HPP
#define EFFECTS_ROTATION_HPP

#include "../core/defs.hpp"
#include "../core/effect.hpp"

TPL(Oriented) struct Rotation : public Effect<Oriented> {
    float duration;
    Quat orientation;
    Rotation(float duration, Quat orientation)
        : duration(duration)
        , orientation(orientation)
    {}
    virt is_expired() const -> bool {
        return duration <= 0;
    }
    virt apply(Oriented& target, float delta) const -> void {
        target.orientation = glm::mix(target.orientation, orientation, delta / duration);
    }
    virt update(float delta) -> void {
        duration -= delta;
    }
};

struct Cube;
CHECK_EFFECT(Rotation, Rotation<Cube>);

TPL(Oriented) func rotation(float duration, Quat orientation, Pool& pool) -> WrapEffectTerm<Oriented> {
    return WrapEffectTerm<Oriented>(new (pool.malloc()) Rotation<Oriented>(duration, orientation));
}

#endif EFFECTS_ROTATION_HPP