#pragma once

#include "GameObject.hpp"
#include "Module.h"
#include "game_math/Vector.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>

/**
 * @brief Scene class represents scene in game
 * with Size dimensions and Type as base type
 */

template <std::size_t Size, typename Type>
class Scene : public Module {
public:
    Scene(const game_math::Vector<Size, Type>& size);

    auto getRootObject() const -> const std::shared_ptr<GameObject<Size, Type>>&;
    auto setRootObject(const std::shared_ptr<GameObject<Size, Type>>& value) -> void;
    auto getSize() const -> const game_math::Vector<Size, Type>&;

private:
    std::shared_ptr<GameObject<Size, Type>> rootObject;
    game_math::Vector<Size, Type> size;
};

template <std::size_t Size, typename Type>
Scene<Size, Type>::Scene(const game_math::Vector<Size, Type>& size)
    : size(size)
{
    for (const Type& item : size.getData()) {
        if (item <= Type()) {
            throw Exception("size should be positive");
        }
    }
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getRootObject() const -> const std::shared_ptr<GameObject<Size, Type>>&
{
    return rootObject;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::setRootObject(const std::shared_ptr<GameObject<Size, Type>>& value) -> void
{
    rootObject = value;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getSize() const -> const game_math::Vector<Size, Type>&
{
    return size;
}
