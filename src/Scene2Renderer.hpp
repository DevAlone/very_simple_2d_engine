#pragma once

#include "Exception.h"
#include "GameObject.hpp"
#include "Module.h"
#include "MovableGameObject.hpp"
#include "SDL2/SDL.h"
#include "SDLWindow.h"
#include "Scene.hpp"
#include "game_math/Vector.hpp"

#include <memory>

/**
 * @brief renders scenes with 2 dimensional objects
 */
template <typename Type>
class Scene2Renderer : public Module {
public:
    Scene2Renderer(
        const std::shared_ptr<Scene<2, Type>>& scene,
        const std::shared_ptr<SDLWindow>& sdlWindow);

    virtual void processFrame(int32_t deltaTime) override;

private:
    void renderObject(const std::shared_ptr<GameObject<2, Type>>& object) const;

    std::shared_ptr<Scene<2, Type>> scene;
    std::shared_ptr<SDLWindow> sdlWindow;
};

template <typename T>
Scene2Renderer<T>::Scene2Renderer(
    const std::shared_ptr<Scene<2, T>>& scene,
    const std::shared_ptr<SDLWindow>& sdlWindow)
    : scene(scene)
    , sdlWindow(sdlWindow)
{
    if (!scene || !sdlWindow) {
        throw Exception("Initialize scene and window first");
    }
}

template <typename Type>
void Scene2Renderer<Type>::processFrame(int32_t)
{
    SDL_Renderer* renderer = sdlWindow->getSdlRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (scene) {
        renderObject(scene->getRootObject());
    }

    SDL_RenderPresent(renderer);
}

template <typename Type>
void Scene2Renderer<Type>::renderObject(const std::shared_ptr<GameObject<2, Type>>& object) const
{
    if (!object)
        return;

    if (auto movableObject = std::dynamic_pointer_cast<MovableGameObject<2, Type>>(object)) {
        SDL_Renderer* renderer = sdlWindow->getSdlRenderer();
        // TODO: render
        auto position = movableObject->getPosition();
        auto size = movableObject->getSize();

        // SDL_QueryTexture(texture, nullptr, &width, &height);
        SDL_Rect rect;
        rect.x = static_cast<int>(position[0]);
        rect.y = static_cast<int>(sdlWindow->getHeight())
            - static_cast<int>(position[1])
            - static_cast<int>(size[1]);

        rect.w = static_cast<int>(size[0]);
        rect.h = static_cast<int>(size[1]);

        if (object->getTexture()) {
            SDL_RenderCopy(
                renderer,
                object->getTexture(),
                nullptr,
                &rect);

        } else {
            SDL_SetRenderDrawColor(
                renderer,
                movableObject->getColor()[0],
                movableObject->getColor()[1],
                movableObject->getColor()[2],
                SDL_ALPHA_OPAQUE);

            SDL_RenderFillRect(
                renderer,
                &rect);
        }
    }

    for (const auto& child : object->getChildren()) {
        renderObject(child);
    }
}
