#pragma once

#include "GameObject.hpp"
#include "Module.h"
#include "MovableGameObject.hpp"
#include "game_math/Vector.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

/**
 * @brief Scene class represents scene in game
 * with Size dimensions and Type as base type
 */

template <std::size_t Size, typename Type>
class Scene : public Module {
    template <std::size_t S, typename T>
    friend class MovableGameObject;

public:
    Scene(const game_math::Vector<Size, Type>& size);

    auto getRootObject() const -> const std::shared_ptr<GameObject<Size, Type>>&;
    static auto setRootObject(
        const std::shared_ptr<Scene<Size, Type>>& scene,
        const std::shared_ptr<GameObject<Size, Type>>& value) -> void;

    auto getSize() const -> const game_math::Vector<Size, Type>&;
    void subscribeOnObjectPositionChanged(
        std::function<void(const MovableGameObject<Size, Type>*,
            const game_math::Vector<Size, Type>&)>
            callback);

private:
    // void removeObjects(const std::shared_ptr<GameObject<Size, Type>>& object);
    void notifyOnObjectPositionChanged(
        const MovableGameObject<Size, Type>* object,
        const game_math::Vector<Size, Type>& oldPosition);

    // std::vector<std::shared_ptr<GameObject<Size, Type>>> objects;
    std::shared_ptr<GameObject<Size, Type>> rootObject;

    const game_math::Vector<Size, Type> size;

    std::vector<std::function<void(const MovableGameObject<Size, Type>*,
        const game_math::Vector<Size, Type>&)>>
        onObjectPositionChangedSubscriptions;
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
auto Scene<Size, Type>::setRootObject(
    const std::shared_ptr<Scene<Size, Type>>& scene,
    const std::shared_ptr<GameObject<Size, Type>>& value) -> void
{
    scene->rootObject = value;
    scene->rootObject->scene = scene;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getSize() const -> const game_math::Vector<Size, Type>&
{
    return size;
}

template <std::size_t Size, typename Type>
void Scene<Size, Type>::subscribeOnObjectPositionChanged(
    std::function<void(const MovableGameObject<Size, Type>*,
        const game_math::Vector<Size, Type>&)>
        callback)
{
    onObjectPositionChangedSubscriptions.push_back(callback);
}

template <std::size_t Size, typename Type>
void Scene<Size, Type>::notifyOnObjectPositionChanged(
    const MovableGameObject<Size, Type>* object,
    const game_math::Vector<Size, Type>& oldPosition)
{
    for (const auto& function : onObjectPositionChangedSubscriptions) {
        function(object, oldPosition);
    }
}

/*template <std::size_t Size, typename Type>
void Scene<Size, Type>::removeObjects(const std::shared_ptr<GameObject<Size, Type>>& object)
{
    if (!object) {
        return;
    }

    objects.erase(
        std::remove(objects.begin(), objects.end(), object),
        objects.end());

    for (const auto& child : object->getChildren()) {
        removeObjects(child);
    }
}*/
