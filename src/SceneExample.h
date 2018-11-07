#ifndef SCENEEXAMPLE_H
#define SCENEEXAMPLE_H

class SDLWindow;
class GameObject;
class InputProcessor;

#include "Scene.h"

class SceneExample : public Scene {
public:
    SceneExample(
        const std::shared_ptr<SDLWindow>& window,
        const std::shared_ptr<InputProcessor>& inputProcessor);

private:
    std::shared_ptr<GameObject> rootObject;
    std::shared_ptr<GameObject> mario;
    const float moveCoefficient = 50;
};

#endif // SCENEEXAMPLE_H
