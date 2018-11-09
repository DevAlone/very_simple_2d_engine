#pragma once

#include "GameObject.hpp"
#include "MovableGameObject.hpp"
#include "game_math/Vector.hpp"

#include <cstddef>

template <std::size_t SIZE, typename Type>
class PhysicsGameObject : public MovableGameObject<SIZE, Type> {
    template <std::size_t _SIZE, typename _Type>
    friend class PhysicsProcessor;

public:
    PhysicsGameObject(
        const game_math::Vector<SIZE, Type>& position,
        const game_math::Vector<SIZE, Type>& size,
        Type mass);

    auto getSpeed() const -> const game_math::Vector<SIZE, Type>&;
    auto addForce(const game_math::Vector<SIZE, Type>& force) -> void;

    const game_math::Vector<SIZE, Type>& getAcceleration() const;

private:
    auto getSpeedRef() -> game_math::Vector<SIZE, Type>&;
    auto setSpeed(const game_math::Vector<SIZE, Type>& value) -> void;
    auto setAcceleration(const game_math::Vector<SIZE, Type>& value) -> void;

    Type mass;
    game_math::Vector<SIZE, Type> speed = game_math::Vector<SIZE, Type>(Type());
    game_math::Vector<SIZE, Type> acceleration = game_math::Vector<SIZE, Type>(Type());
};

using PhysicsGameObject2F = PhysicsGameObject<2, float>;

template <std::size_t SIZE, typename Type>
PhysicsGameObject<SIZE, Type>::PhysicsGameObject(
    const game_math::Vector<SIZE, Type>& position,
    const game_math::Vector<SIZE, Type>& size,
    Type mass)
    : MovableGameObject<SIZE, Type>(position, size)
    , mass(mass)
{
}

template <std::size_t SIZE, typename Type>
auto PhysicsGameObject<SIZE, Type>::getSpeed() const -> const game_math::Vector<SIZE, Type>&
{
    return speed;
}

template <std::size_t SIZE, typename Type>
void PhysicsGameObject<SIZE, Type>::addForce(const game_math::Vector<SIZE, Type>& force)
{
    acceleration += force / mass;
}

template <std::size_t SIZE, typename Type>
const game_math::Vector<SIZE, Type>& PhysicsGameObject<SIZE, Type>::getAcceleration() const
{
    return acceleration;
}

template <std::size_t SIZE, typename Type>
auto PhysicsGameObject<SIZE, Type>::getSpeedRef() -> game_math::Vector<SIZE, Type>&
{
    return speed;
}

template <std::size_t SIZE, typename Type>
auto PhysicsGameObject<SIZE, Type>::setSpeed(const game_math::Vector<SIZE, Type>& value) -> void
{
    speed = value;
}

template <std::size_t SIZE, typename Type>
void PhysicsGameObject<SIZE, Type>::setAcceleration(const game_math::Vector<SIZE, Type>& value)
{
    acceleration = value;
}
