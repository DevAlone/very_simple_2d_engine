#ifndef SCENEEXAMPLE_H
#define SCENEEXAMPLE_H

class SDLWindow;
class InputProcessor;

#include "CollisionsProcessor.hpp"
#include "GameObject.hpp"
#include "PhysicsGameObject.hpp"
#include "Scene.hpp"

#include <vector>

class SceneExample : public Scene<2, float> {
public:
    SceneExample(
        const std::shared_ptr<SDLWindow>& window,
        const std::shared_ptr<InputProcessor>& inputProcessor);

    virtual void processFrame(int32_t deltaTime) override;

    void setCollisionsProcessor(
        const std::shared_ptr<CollisionsProcessor<2, float>>& collisionsProcessor);

private:
    std::shared_ptr<GameObject2F> rootObject;
    std::shared_ptr<PhysicsGameObject2F> mario;
    std::shared_ptr<PhysicsGameObject2F> marioChild;
    const float moveCoefficient = 10000;
    const float cellSize = 50;
    // in microseconds
    const size_t objectCreatingPeriod = 1000 * 1000 * 1;
    size_t lastObjectCreationgTime = 0;
    std::vector<std::shared_ptr<MovableGameObject2F>> objects;
};

#endif // SCENEEXAMPLE_H
