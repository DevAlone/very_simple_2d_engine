#ifndef SCENEEXAMPLE_H
#define SCENEEXAMPLE_H

class SDLWindow;
class GameObject;

#include "Scene.h"
// #include "SDLWindow.h"

class SceneExample : public Scene {
public:
    SceneExample(const std::shared_ptr<SDLWindow>& window);

private:
    std::shared_ptr<GameObject> rootObject;
    std::shared_ptr<GameObject> mario;
};

#endif // SCENEEXAMPLE_H
