#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Module.h"
#include "game_math/Vector.hpp"

#include <memory>

class Scene;
class GameObject;
class SDLWindow;

class SceneRenderer : public Module {
public:
    SceneRenderer(
        const std::shared_ptr<Scene>& scene,
        const std::shared_ptr<SDLWindow>& sdlWindow);

    virtual void processFrame(int32_t deltaTime) override;

private:
    void renderObject(
        const std::shared_ptr<GameObject>& object,
        const game_math::Vector2F& parentCoordinates) const;

    std::shared_ptr<Scene> scene;
    std::shared_ptr<SDLWindow> sdlWindow;
};

#endif // SCENERENDERER_H
