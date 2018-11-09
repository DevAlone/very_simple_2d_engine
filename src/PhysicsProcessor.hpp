#pragma once

#include "Exception.h"
#include "GameObject.hpp"
#include "Module.h"
#include "PhysicsGameObject.hpp"
#include "Scene.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>

/**
 * @brief processes physics for object inside the scene
 * with SIZE dimensions and Type as base type
 */
template <std::size_t SIZE, typename Type>
class PhysicsProcessor : public Module {
public:
    PhysicsProcessor(const std::shared_ptr<Scene<SIZE, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override;

private:
    void processPhysicsForObject(
        int32_t deltaTime,
        const std::shared_ptr<GameObject<SIZE, Type>>& object);

    const std::shared_ptr<Scene<SIZE, Type>> scene;
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
void PhysicsProcessor<SIZE, Type>::processPhysicsForObject(
    int32_t deltaTime, const std::shared_ptr<GameObject<SIZE, Type>>& object)
{
    if (!object) {
        return;
    }

    if (auto physicsObject = std::dynamic_pointer_cast<PhysicsGameObject<SIZE, Type>>(object)) {
        physicsObject->getPositionRef() += physicsObject->getSpeed() * (Type(deltaTime) / Type(1000000));

        game_math::Vector<SIZE, Type>& position = physicsObject->getPositionRef();

        // limit to scene area
        for (size_t i = 0; i < SIZE; ++i) {
            if (position[i] < Type()) {
                position[i] = Type();
            } else if (position[i] > scene->getSize()[i]) {
                position[i] = scene->getSize()[i];
            }
        }
    }

    for (const auto& child : object->getChildren()) {
        processPhysicsForObject(deltaTime, child);
    }
}
