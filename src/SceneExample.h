#ifndef SCENEEXAMPLE_H
#define SCENEEXAMPLE_H

class SDLWindow;
class InputProcessor;

#include "GameObject.hpp"
#include "PhysicsGameObject.hpp"
#include "Scene.hpp"

class SceneExample : public Scene<2, float> {
public:
    SceneExample(
        const std::shared_ptr<SDLWindow>& window,
        const std::shared_ptr<InputProcessor>& inputProcessor);

private:
    std::shared_ptr<GameObject2F> rootObject;
    std::shared_ptr<PhysicsGameObject2F> mario;
    const float moveCoefficient = 50;
};

#endif // SCENEEXAMPLE_H
