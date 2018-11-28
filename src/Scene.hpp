#pragma once

#include "GameObject.hpp"
#include "Module.h"
#include "MovableGameObject.hpp"
#include "boost/signals2.hpp"
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

    using OnObjectPositionChangedSignal
        = boost::signals2::signal<void(MovableGameObject<Size, Type>*,
            const game_math::Vector<Size, Type>&)>;

    using OnObjectRemovedSignal = boost::signals2::signal<void(GameObject<Size, Type>*)>;

public:
    Scene(const game_math::Vector<Size, Type>& size);

    static auto setRootObject(
        const std::shared_ptr<Scene<Size, Type>>& scene,
        const std::shared_ptr<GameObject<Size, Type>>& value) -> void;

    auto getRootObject() const -> const std::shared_ptr<GameObject<Size, Type>>&;
    auto getSize() const -> const game_math::Vector<Size, Type>&;
    auto getOnObjectPositionChangedSignal() -> OnObjectPositionChangedSignal&;
    auto getOnObjectRemovedSignal() -> OnObjectRemovedSignal&;

private:
    // void removeObjects(const std::shared_ptr<GameObject<Size, Type>>& object);

    // std::vector<std::shared_ptr<GameObject<Size, Type>>> objects;
    std::shared_ptr<GameObject<Size, Type>> rootObject;

    const game_math::Vector<Size, Type> size;

    OnObjectPositionChangedSignal onObjectPositionChangedSignal;
    OnObjectRemovedSignal onObjectRemovedSignal;
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
    if (scene->rootObject) {
        scene->getOnObjectRemovedSignal()(scene->rootObject.get());
        scene->rootObject->scene.reset();
    }
    scene->rootObject = value;
    scene->rootObject->scene = scene;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getSize() const -> const game_math::Vector<Size, Type>&
{
    return size;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getOnObjectPositionChangedSignal() -> OnObjectPositionChangedSignal&
{
    return onObjectPositionChangedSignal;
}

template <std::size_t Size, typename Type>
auto Scene<Size, Type>::getOnObjectRemovedSignal() -> OnObjectRemovedSignal&
{
    return onObjectRemovedSignal;
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
