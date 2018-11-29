#pragma once

#include "Core.h"
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
 * with nDimensions dimensions and BaseType as base type
 */

template <std::size_t nDimensions, typename BaseType>
class Scene : public Module {
    template <std::size_t S, typename T>
    friend class GameObject;

    template <std::size_t S, typename T>
    friend class MovableGameObject;

    using OnObjectPositionChangedSignal
        = boost::signals2::signal<void(MovableGameObject<nDimensions, BaseType>*,
            const game_math::Vector<nDimensions, BaseType>&)>;

    using OnObjectRemovedSignal = boost::signals2::signal<
        void(GameObject<nDimensions, BaseType>*)>;

    using GameObjectPtr = std::shared_ptr<GameObject<nDimensions, BaseType>>;

public:
    Scene(const game_math::Vector<nDimensions, BaseType>& size);

    static auto setRootObject(
        const std::shared_ptr<Scene<nDimensions, BaseType>>& scene,
        const GameObjectPtr& value) -> void;

    auto getRootObject() const -> const GameObjectPtr&;
    auto getSize() const -> const game_math::Vector<nDimensions, BaseType>&;

    void subscribeOnObjectPositionChanged(
        std::function<typename OnObjectPositionChangedSignal::signature_type> callback);

    void subscribeOnObjectRemoved(
        std::function<typename OnObjectRemovedSignal::signature_type> callback);

    void printObjectsTree() const;

    virtual void onModuleCreation() override;

private:
    void printObjectsTreeHelper(
        const std::string& prefix,
        const GameObjectPtr& object, bool isLast = false) const;

    GameObjectPtr rootObject;

    const game_math::Vector<nDimensions, BaseType> size;

    OnObjectPositionChangedSignal onObjectPositionChangedSignal;
    OnObjectRemovedSignal onObjectRemovedSignal;
};

template <std::size_t nDimensions, typename BaseType>
Scene<nDimensions, BaseType>::Scene(const game_math::Vector<nDimensions, BaseType>& size)
    : size(size)
{
    for (const BaseType& item : size.getData()) {
        if (item <= BaseType()) {
            throw Exception("size should be positive");
        }
    }
}

template <std::size_t nDimensions, typename BaseType>
void Scene<nDimensions, BaseType>::onModuleCreation()
{
}

template <std::size_t nDimensions, typename BaseType>
auto Scene<nDimensions, BaseType>::getRootObject() const -> const GameObjectPtr&
{
    return rootObject;
}

template <std::size_t nDimensions, typename BaseType>
auto Scene<nDimensions, BaseType>::setRootObject(
    const std::shared_ptr<Scene<nDimensions, BaseType>>& scene,
    const GameObjectPtr& value) -> void
{
    if (scene->rootObject) {
        scene->rootObject->removeSceneRecursively();
    }
    scene->rootObject = value;
    scene->rootObject->setSceneRecursively(scene);
}

template <std::size_t nDimensions, typename BaseType>
auto Scene<nDimensions, BaseType>::getSize() const -> const game_math::Vector<nDimensions, BaseType>&
{
    return size;
}

template <std::size_t nDimensions, typename BaseType>
void Scene<nDimensions, BaseType>::subscribeOnObjectPositionChanged(
    std::function<typename OnObjectPositionChangedSignal::signature_type> callback)
{
    onObjectPositionChangedSignal.connect(callback);
}

template <std::size_t nDimensions, typename BaseType>
void Scene<nDimensions, BaseType>::subscribeOnObjectRemoved(
    std::function<typename OnObjectRemovedSignal::signature_type> callback)
{
    onObjectRemovedSignal.connect(callback);
}

template <std::size_t nDimensions, typename BaseType>
void Scene<nDimensions, BaseType>::printObjectsTree() const
{
    printObjectsTreeHelper("", rootObject);
}

template <std::size_t nDimensions, typename BaseType>
void Scene<nDimensions, BaseType>::printObjectsTreeHelper(
    const std::string& prefix,
    const Scene::GameObjectPtr& object,
    bool isLast) const
{
    if (!object) {
        return;
    }

    std::cout << prefix;
    std::cout << (!isLast ? "├──" : "└──");

    std::cout << object << std::endl;

    for (auto it = object->getChildren().begin(); it != object->getChildren().end(); ++it) {
        auto next = it;
        ++next;
        printObjectsTreeHelper(
            prefix + (!isLast ? "|   " : "    "),
            *it,
            next == object->getChildren().end());
    }
}
