#include "PhysicsProcessor.h"
#include "Exception.h"
#include "GameObject.h"
#include "Scene.h"

PhysicsProcessor::PhysicsProcessor(
    const std::shared_ptr<Scene>& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("You gotta initialize scene first");
    }
}

void PhysicsProcessor::processFrame(int32_t deltaTime)
{
    processPhysicsForObject(deltaTime, scene->getRootObject());
}

void PhysicsProcessor::processPhysicsForObject(int32_t deltaTime, const std::shared_ptr<GameObject>& object)
{
    if (!object) {
        return;
    }

    object->getPositionRef() += object->getSpeed() * (float(deltaTime) / 1000000.0f);

    for (const auto& child : object->getChildren()) {
        processPhysicsForObject(deltaTime, child);
    }
}
