#pragma once

#include "GameObject.hpp"
#include "game_math/Vector.hpp"

#include <cstddef>

template <std::size_t Size, typename Type>
class MovableGameObject : public GameObject<Size, Type> {
public:
    MovableGameObject(
        const game_math::Vector<Size, Type>& position = { 0, 0 },
        const game_math::Vector<Size, Type>& size = { 0, 0 });

    auto getPosition() const -> const game_math::Vector<Size, Type>&;
    auto getPositionRef() -> game_math::Vector<Size, Type>&;
    auto setPosition(const game_math::Vector<Size, Type>& value) -> void;
    auto getSize() const -> const game_math::Vector<Size, Type>&;
    auto setSize(const game_math::Vector<Size, Type>& value) -> void;

protected:
    game_math::Vector<Size, Type> position;
    game_math::Vector<Size, Type> size;
};

using MovableGameObject2F = MovableGameObject<2, float>;

template <std::size_t Size, typename Type>
MovableGameObject<Size, Type>::MovableGameObject(
    const game_math::Vector<Size, Type>& position,
    const game_math::Vector<Size, Type>& size)
    : position(position)
    , size(size)
{
}

template <std::size_t Size, typename Type>
auto MovableGameObject<Size, Type>::getPositionRef() -> game_math::Vector<Size, Type>&
{
    return position;
}

template <std::size_t Size, typename Type>
const game_math::Vector<Size, Type>& MovableGameObject<Size, Type>::getPosition() const
{
    return position;
}

template <std::size_t Size, typename Type>
void MovableGameObject<Size, Type>::setPosition(const game_math::Vector<Size, Type>& value)
{
    position = value;
}

template <std::size_t Size, typename Type>
const game_math::Vector<Size, Type>& MovableGameObject<Size, Type>::getSize() const
{
    return size;
}

template <std::size_t Size, typename Type>
void MovableGameObject<Size, Type>::setSize(const game_math::Vector<Size, Type>& value)
{
    size = value;
}
