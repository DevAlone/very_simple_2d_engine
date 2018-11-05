#include "SceneExample.h"
#include "Exception.h"
#include "GameObject.h"
#include "SDLWindow.h"
#include "SDL_keycode.h"

#include <iostream>

SceneExample::SceneExample(const std::shared_ptr<SDLWindow>& window)
{
    if (!window) {
        // TODO: refactor!
        throw Exception("Trying to create scene without window");
    }
    rootObject = std::make_shared<GameObject>(
        Vector2F(0, 0), Vector2F(640, 480));

    mario = std::make_shared<GameObject>(
        Vector2F(50, 50), Vector2F(50, 50));

    mario->setTextureFromFile(
        "/home/user/projects/sdl_example/mario.bmp",
        window->getSdlRenderer());

    /*auto marioChild = std::make_shared<GameObject>(
        Vector2F(50, 50), Vector2F(50, 50));

    marioChild->setTextureFromFile(
        "/home/user/projects/sdl_example/mario.jpg",
        window);

    mario->addChild(marioChild);*/

    rootObject->addChild(mario);
    setRootObject(rootObject);

    const int moveCoefficient = 50;

    mario->addSpeed({ 0, -9.8f });

    window->subscribeOnKeyPress(SDLK_SPACE, [this]() {
        mario->getPositionRef().getYRef() += moveCoefficient;
    });

    window->subscribeOnKeyPress(SDLK_w, [this]() {
        mario->addSpeed({ 0, moveCoefficient });
    });
    window->subscribeOnKeyRelease(SDLK_w, [this]() {
        mario->addSpeed({ 0, -moveCoefficient });
    });

    window->subscribeOnKeyPress(SDLK_s, [this] {
        mario->addSpeed({ 0, -moveCoefficient });
    });
    window->subscribeOnKeyRelease(SDLK_s, [this]() {
        mario->addSpeed({ 0, moveCoefficient });
    });

    window->subscribeOnKeyPress(SDLK_a, [this]() {
        mario->addSpeed({ -moveCoefficient, 0 });
    });
    window->subscribeOnKeyRelease(SDLK_a, [this]() {
        mario->addSpeed({ moveCoefficient, 0 });
    });

    window->subscribeOnKeyPress(SDLK_d, [this]() {
        mario->addSpeed({ moveCoefficient, 0 });
    });
    window->subscribeOnKeyRelease(SDLK_d, [this]() {
        mario->addSpeed({ -moveCoefficient, 0 });
    });
}
