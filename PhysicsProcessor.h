#ifndef PHYSICSPROCESSOR_H
#define PHYSICSPROCESSOR_H

#include "Module.h"

#include <cstdint>
#include <memory>

class Scene;
class GameObject;

class PhysicsProcessor : public Module {
public:
    PhysicsProcessor(const std::shared_ptr<Scene>& scene);

    virtual void processFrame(int32_t deltaTime) override;

private:
    void processPhysicsForObject(
        int32_t deltaTime,
        const std::shared_ptr<GameObject>& object);

    const std::shared_ptr<Scene> scene;
};

#endif // PHYSICSPROCESSOR_H
