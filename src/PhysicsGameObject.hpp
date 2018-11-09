#pragma once

#include "GameObject.hpp"
#include "MovableGameObject.hpp"
#include "game_math/Vector.hpp"

#include <cstddef>

template <std::size_t Size, typename Type>
class PhysicsGameObject : public MovableGameObject<Size, Type> {
public:
    PhysicsGameObject(
        const game_math::Vector<Size, Type>& position,
        const game_math::Vector<Size, Type>& size,
        Type mass);

    auto getSpeed() const -> const game_math::Vector<Size, Type>&;
    auto getSpeedRef() -> game_math::Vector<Size, Type>&;
    auto setSpeed(const game_math::Vector<Size, Type>& value) -> void;

private:
    Type mass;
    game_math::Vector<Size, Type> speed;
};

using PhysicsGameObject2F = PhysicsGameObject<2, float>;

template <std::size_t Size, typename Type>
PhysicsGameObject<Size, Type>::PhysicsGameObject(
    const game_math::Vector<Size, Type>& position,
    const game_math::Vector<Size, Type>& size,
    Type mass)
    : MovableGameObject<Size, Type>(position, size)
    , mass(mass)
{
}

template <std::size_t Size, typename Type>
auto PhysicsGameObject<Size, Type>::getSpeed() const -> const game_math::Vector<Size, Type>&
{
    return speed;
}

template <std::size_t Size, typename Type>
auto PhysicsGameObject<Size, Type>::getSpeedRef() -> game_math::Vector<Size, Type>&
{
    return speed;
}

template <std::size_t Size, typename Type>
auto PhysicsGameObject<Size, Type>::setSpeed(const game_math::Vector<Size, Type>& value) -> void
{
    speed = value;
}
