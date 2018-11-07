#include "SceneExample.h"
#include "Exception.h"
#include "GameObject.h"
#include "InputProcessor.h"
#include "SDLWindow.h"
#include "SDL_keycode.h"
#include "game_math/Vector.hpp"

#include <iostream>

using namespace game_math;

SceneExample::SceneExample(
    const std::shared_ptr<SDLWindow>& window,
    const std::shared_ptr<InputProcessor>& inputProcessor)
{
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

    mario->addSpeed({ 0, -9.8f });

    inputProcessor->subscribeOnKey(
        Key::W,
        [this]() {
            mario->addSpeed({ 0, moveCoefficient });
        },
        [this]() {
            mario->addSpeed({ 0, -moveCoefficient });
        });

    inputProcessor->subscribeOnKey(
        Key::S,
        [this]() {
            mario->addSpeed({ 0, -moveCoefficient });
        },
        [this]() {
            mario->addSpeed({ 0, moveCoefficient });
        });

    inputProcessor->subscribeOnKey(
        Key::D,
        [this]() {
            mario->addSpeed({ moveCoefficient, 0 });
        },
        [this]() {
            mario->addSpeed({ -moveCoefficient, 0 });
        });

    inputProcessor->subscribeOnKey(
        Key::A,
        [this]() {
            mario->addSpeed({ -moveCoefficient, 0 });
        },
        [this]() {
            mario->addSpeed({ moveCoefficient, 0 });
        });
}
