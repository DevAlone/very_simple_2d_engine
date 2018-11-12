#pragma once

#include "Exception.h"
#include "GameObject.hpp"
#include "Module.h"
#include "PhysicsGameObject.hpp"
#include "Scene.hpp"
#include "game_math/game_math.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief processes physics for objects inside the scene
 * with SIZE dimensions and Type as base type
 */
template <std::size_t SIZE, typename Type>
class PhysicsProcessor : public Module {
public:
    PhysicsProcessor(const std::shared_ptr<Scene<SIZE, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override;
    void addAccelerationForAllObjects(const game_math::Vector<SIZE, Type>& acceleration);

private:
    void processPhysicsForObject(
        int32_t deltaTime,
        const std::shared_ptr<GameObject<SIZE, Type>>& object);

    const std::shared_ptr<Scene<SIZE, Type>> scene;
    game_math::Vector<SIZE, Type> accelerations = game_math::Vector<SIZE, Type>(Type());
};

template <std::size_t SIZE, typename Type>
PhysicsProcessor<SIZE, Type>::PhysicsProcessor(
    const std::shared_ptr<Scene<SIZE, Type>>& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("You gotta initialize scene first");
    }
}

template <std::size_t SIZE, typename Type>
void PhysicsProcessor<SIZE, Type>::processFrame(int32_t deltaTime)
{
    processPhysicsForObject(deltaTime, scene->getRootObject());
}

template <std::size_t SIZE, typename Type>
void PhysicsProcessor<SIZE, Type>::addAccelerationForAllObjects(
    const game_math::Vector<SIZE, Type>& acceleration)
{
    accelerations += acceleration;
}

#include <iostream>
#include <unistd.h>

template <std::size_t SIZE, typename Type>
void PhysicsProcessor<SIZE, Type>::processPhysicsForObject(
    int32_t deltaTime, const std::shared_ptr<GameObject<SIZE, Type>>& object)
{
    if (!object) {
        return;
    }

    Type k = Type(deltaTime) / Type(1000000);

    if (auto physicsObject = std::dynamic_pointer_cast<PhysicsGameObject<SIZE, Type>>(object)) {
        // apply global accelerations
        physicsObject->getSpeedRef() += accelerations * k;

        // apply objects accelerations

        // physicsObject->getSpeedRef() += physicsObject->getAcceleration() * k;

        physicsObject->getPositionRef() += physicsObject->getSpeed() * k;

        // limit to scene area
        game_math::Vector<SIZE, Type>& position = physicsObject->getPositionRef();

        for (size_t i = 0; i < SIZE; ++i) {
            bool hit = false;

            if (position[i] < Type()) {
                position[i] = Type();
                hit = true;
            } else if (position[i] > scene->getSize()[i]) {
                position[i] = scene->getSize()[i];
                hit = true;
            }

            if (hit) {
                // physicsObject->getSpeedRef()[i] = Type();
                physicsObject->getSpeedRef() = game_math::Vector<SIZE, Type>(Type());

                physicsObject->getSpeedRef() += physicsObject->getAcceleration() * k;
            }
        }
    }

    for (const auto& child : object->getChildren()) {
        processPhysicsForObject(deltaTime, child);
    }
}
