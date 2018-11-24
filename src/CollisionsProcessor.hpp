#pragma once

#include "Module.h"
#include "MovableGameObject.hpp"
#include "Scene.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <std::size_t nDimensions, typename Type>
class CollisionsProcessor : public Module {
    using MovableObjectType = MovableGameObject<nDimensions, Type>;
    using MovableObjectPtrType = std::shared_ptr<MovableObjectType>;

public:
    CollisionsProcessor(const std::shared_ptr<Scene<nDimensions, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override;

    /**
     * @brief subscribeOnCollision - subscribe on every collision with object
     * @param object - object to watch for collisions
     * @param onCollisionStarted - callback which called when collision started
     * @param onCollisionFinished - callback which called when collision finished
     */
    void subscribeOnCollision(
        const MovableObjectPtrType& object,
        std::function<void(const MovableObjectPtrType&)> onCollisionStarted,
        std::function<void(const MovableObjectPtrType&)> onCollisionFinished);

    /**
     * @brief subscribeOnCollision - subscribe on every collision between two objects
     * @param object1 - first object to watch for collisions
     * @param object2 - second object to watch for collisions
     * @param onCollisionStarted - callback which called when collision started
     * @param onCollisionFinished - callback which called when collision finished
     */
    void subscribeOnCollision(
        const MovableObjectPtrType& object1,
        const MovableObjectPtrType& object2,
        std::function<void()> onCollisionStarted,
        std::function<void()> onCollisionFinished);

private:
    void processCollision(
        const MovableObjectPtrType& object1,
        const MovableObjectPtrType& object2,
        bool isStarted);

    std::shared_ptr<Scene<nDimensions, Type>> scene;
    std::unordered_map<
        MovableObjectPtrType,
        std::unordered_set<MovableObjectPtrType>>
        collisionsMap;

    std::unordered_map<
        MovableObjectPtrType,
        std::vector<std::function<void(const MovableObjectPtrType&)>>>
        onObjectCollisionStartedSubscriptions;

    std::unordered_map<
        MovableObjectPtrType,
        std::vector<std::function<void(const MovableObjectPtrType&)>>>
        onObjectCollisionFinishedSubscriptions;

    struct PairHash {
        template <typename Type1, typename Type2>
        std::size_t operator()(const std::pair<Type1, Type2>& pair) const
        {
            return std::hash<Type1>()(pair.first) ^ std::hash<Type2>()(pair.second);
        }
    };

    std::unordered_map<
        std::pair<MovableObjectPtrType, MovableObjectPtrType>,
        std::vector<std::function<void()>>,
        PairHash>
        onObjectsCollisionStartedSubscriptions;

    std::unordered_map<
        std::pair<MovableObjectPtrType, MovableObjectPtrType>,
        std::vector<std::function<void()>>,
        PairHash>
        onObjectsCollisionFinishedSubscriptions;

    void collectMovables(
        const std::shared_ptr<GameObject<nDimensions, Type>>& object,
        std::vector<MovableObjectPtrType>& outMovables);
};

template <std::size_t nDimensions, typename Type>
CollisionsProcessor<nDimensions, Type>::CollisionsProcessor(
    const std::shared_ptr<Scene<nDimensions, Type>>& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("bad scene");
    }
}

template <std::size_t nDimensions, typename Type>
void CollisionsProcessor<nDimensions, Type>::processFrame(int32_t)
{
    std::unordered_map<
        MovableObjectPtrType,
        std::unordered_set<MovableObjectPtrType>>
        newCollisionsMap;

    std::vector<MovableObjectPtrType> movables;

    collectMovables(scene->getRootObject(), movables);

    // TODO: optimize
    for (size_t i = 0; i < movables.size(); ++i) {
        for (size_t j = i + 1; j < movables.size(); ++j) {
            auto& movable1 = movables[i];
            auto& movable2 = movables[j];
            bool collisionFound = true;

            for (size_t n = 0; n < nDimensions; ++n) {
                if (
                    movable1->getPosition()[n] + movable1->getSize()[n] < movable2->getPosition()[n]
                    || movable1->getPosition()[n] > movable2->getPosition()[n] + movable2->getSize()[n]) {

                    collisionFound = false;
                    break;
                }
            }

            if (collisionFound) {
                newCollisionsMap[movable1].insert(movable2);
                newCollisionsMap[movable2].insert(movable1);

                auto it = collisionsMap.find(movable1);
                if (it == collisionsMap.end() || it->second.find(movable2) == it->second.end()) {
                    processCollision(movable1, movable2, true);
                }
            }
        }
    }

    for (const auto& [object1, objectsSet] : collisionsMap) {
        if (newCollisionsMap.find(object1) == newCollisionsMap.end()) {
            // someone went out from collision
            for (const auto& object2 : objectsSet) {
                processCollision(object1, object2, false);
                collisionsMap.erase(object2);
            }
        }
    }

    collisionsMap = newCollisionsMap;
}

template <std::size_t nDimensions, typename Type>
void CollisionsProcessor<nDimensions, Type>::subscribeOnCollision(
    const CollisionsProcessor::MovableObjectPtrType& object,
    std::function<void(const CollisionsProcessor::MovableObjectPtrType&)> onCollisionStarted,
    std::function<void(const CollisionsProcessor::MovableObjectPtrType&)> onCollisionFinished)
{
    onObjectCollisionStartedSubscriptions[object].push_back(onCollisionStarted);
    onObjectCollisionFinishedSubscriptions[object].push_back(onCollisionFinished);
}

template <std::size_t nDimensions, typename Type>
void CollisionsProcessor<nDimensions, Type>::subscribeOnCollision(
    const CollisionsProcessor::MovableObjectPtrType& object1,
    const CollisionsProcessor::MovableObjectPtrType& object2,
    std::function<void()> onCollisionStarted,
    std::function<void()> onCollisionFinished)
{
    onObjectsCollisionStartedSubscriptions[std::make_pair(object1, object2)].push_back(
        onCollisionStarted);
    onObjectsCollisionFinishedSubscriptions[std::make_pair(object1, object2)].push_back(
        onCollisionFinished);
}

template <std::size_t nDimensions, typename Type>
void CollisionsProcessor<nDimensions, Type>::processCollision(
    const MovableObjectPtrType& object1,
    const MovableObjectPtrType& object2,
    bool isStarted)
{
    auto& onObjectCollisionSubscriptions = isStarted ? onObjectCollisionStartedSubscriptions
                                                     : onObjectCollisionFinishedSubscriptions;

    {
        auto it = onObjectCollisionSubscriptions.find(object1);
        if (it != onObjectCollisionSubscriptions.end()) {
            for (const auto& function : it->second) {
                function(object2);
            }
        }
    }
    {
        auto it = onObjectCollisionSubscriptions.find(object2);
        if (it != onObjectCollisionSubscriptions.end()) {
            for (const auto& function : it->second) {
                function(object1);
            }
        }
    }

    auto& onObjectsCollisionSubscriptions = isStarted ? onObjectsCollisionStartedSubscriptions
                                                      : onObjectsCollisionFinishedSubscriptions;

    {
        auto it = onObjectsCollisionSubscriptions.find(std::make_pair(object1, object2));
        if (it == onObjectsCollisionSubscriptions.end()) {
            it = onObjectsCollisionSubscriptions.find(std::make_pair(object2, object1));
        }
        if (it != onObjectsCollisionSubscriptions.end()) {
            for (const auto& function : it->second) {
                function();
            }
        }
    }
}

template <std::size_t nDimensions, typename Type>
void CollisionsProcessor<nDimensions, Type>::collectMovables(
    const std::shared_ptr<GameObject<nDimensions, Type>>& object,
    std::vector<MovableObjectPtrType>& outMovables)
{
    if (!object) {
        return;
    }

    if (auto movableObject = std::dynamic_pointer_cast<MovableObjectType>(
            object)) {
        outMovables.push_back(movableObject);
    }

    for (const auto& child : object->getChildren()) {
        collectMovables(child, outMovables);
    }
}

/*template <typename Type>
class CollisionsProcessor<2, Type> : public Module {
public:
    CollisionsProcessor(const std::shared_ptr<Scene<2, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override
    {
    }
};*/
