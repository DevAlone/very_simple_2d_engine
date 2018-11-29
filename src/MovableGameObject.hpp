#pragma once

#include "GameObject.hpp"
#include "game_math/Vector.hpp"

#include <cstddef>

template <std::size_t nDimensions, typename BaseType>
class MovableGameObject : public GameObject<nDimensions, BaseType> {
    using VectorBaseType = game_math::Vector<nDimensions, BaseType>;

public:
    MovableGameObject(
        const VectorBaseType& position = { 0, 0 },
        const VectorBaseType& size = { 0, 0 });

    auto getPosition() const -> const VectorBaseType&;
    auto setPosition(const VectorBaseType& value) -> void;
    auto getSize() const -> const VectorBaseType&;
    auto setSize(const VectorBaseType& value) -> void;
    auto move(const VectorBaseType& direction) -> void;
    auto moveWithChildren(const VectorBaseType& direction) -> void;

private:
    VectorBaseType position;
    VectorBaseType size;
};

using MovableGameObject2F = MovableGameObject<2, float>;

template <std::size_t nDimensions, typename BaseType>
MovableGameObject<nDimensions, BaseType>::MovableGameObject(
    const VectorBaseType& position,
    const VectorBaseType& size)
    : position(position)
    , size(size)
{
}

template <std::size_t nDimensions, typename BaseType>
auto MovableGameObject<nDimensions, BaseType>::getPosition() const
    -> const VectorBaseType&
{
    return position;
}

template <std::size_t nDimensions, typename BaseType>
void MovableGameObject<nDimensions, BaseType>::setPosition(const VectorBaseType& value)
{
    auto oldPosition = position;
    position = value;
    if (auto scene = this->sceneWeakPtr.lock()) {
        scene->onObjectPositionChangedSignal(this, oldPosition);
    }
}

template <std::size_t nDimensions, typename BaseType>
auto MovableGameObject<nDimensions, BaseType>::getSize() const
    -> const VectorBaseType&
{
    return size;
}

template <std::size_t nDimensions, typename BaseType>
void MovableGameObject<nDimensions, BaseType>::setSize(const VectorBaseType& value)
{
    size = value;
}

template <std::size_t nDimensions, typename BaseType>
void MovableGameObject<nDimensions, BaseType>::move(
    const MovableGameObject::VectorBaseType& direction)
{
    setPosition(getPosition() + direction);
}

template <std::size_t nDimensions, typename BaseType>
void MovableGameObject<nDimensions, BaseType>::moveWithChildren(
    const MovableGameObject::VectorBaseType& direction)
{
    move(direction);
    for (auto& child : this->getChildren()) {
        if (auto movableChild
            = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {

            movableChild->moveWithChildren(direction);
        }
    }
}

template <std::size_t nDimensions, typename BaseBaseType>
std::ostream& operator<<(
    std::ostream& stream,
    const MovableGameObject<nDimensions, BaseBaseType>& obj)
{
    return stream
        << "MovableGameObject { name: \""
        << obj.getName()
        << "\", address: \""
        << &obj
        << ", position: "
        << obj.getPosition()
        << "\" }";
}

template <std::size_t nDimensions, typename BaseBaseType>
std::ostream& operator<<(
    std::ostream& stream,
    const std::shared_ptr<MovableGameObject<nDimensions, BaseBaseType>>& obj)
{
    return stream
        << "MovableGameObjectPtr { name: \""
        << obj->getName()
        << "\", address: \""
        << obj.get()
        << "\" }";
}
