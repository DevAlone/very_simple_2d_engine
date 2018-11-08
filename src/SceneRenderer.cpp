#include "SceneRenderer.h"
#include "Exception.h"
#include "GameObject.h"
#include "SDLWindow.h"
#include "Scene.h"
#include "game_math/Vector.hpp"

#include "SDL2/SDL.h"

using namespace game_math;

SceneRenderer::SceneRenderer(
    const std::shared_ptr<Scene>& scene,
    const std::shared_ptr<SDLWindow>& sdlWindow)
    : scene(scene)
    , sdlWindow(sdlWindow)
{
    if (!scene || !sdlWindow) {
        throw Exception("Initialize scene and window first");
    }
}

void SceneRenderer::processFrame(int32_t)
{
    SDL_Renderer* renderer = sdlWindow->getSdlRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    if (scene) {
        renderObject(scene->getRootObject());
    }

    SDL_RenderPresent(renderer);
}

void SceneRenderer::renderObject(const std::shared_ptr<GameObject>& object) const
{
    if (!object)
        return;

    SDL_Renderer* renderer = sdlWindow->getSdlRenderer();
    // TODO: render
    auto position = object->getPosition();
    auto size = object->getSize();

    // SDL_QueryTexture(texture, nullptr, &width, &height);
    SDL_Rect rect;
    rect.x = static_cast<int>(position.getX());
    rect.y = static_cast<int>(sdlWindow->getHeight())
        - static_cast<int>(position.getY())
        - static_cast<int>(size.getY());

    // rect.y = static_cast<int>(position.getY());

    rect.w = static_cast<int>(size.getX());
    rect.h = static_cast<int>(size.getY());

    if (object->getTexture()) {
        SDL_RenderCopy(
            renderer,
            object->getTexture(),
            nullptr,
            &rect);

        /*SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            SDL_ALPHA_OPAQUE);*/
    } else {
        // TODO: render rectangle
    }

    for (const auto& child : object->getChildren()) {
        renderObject(child);
    }
}
