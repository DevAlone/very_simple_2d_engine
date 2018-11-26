#include "SceneExample.h"
#include "Core.h"

#include "CollisionsProcessor.hpp"
#include "Exception.h"
#include "InputProcessor.h"
#include "MovableGameObject.hpp"
#include "PhysicsGameObject.hpp"
#include "SDLWindow.h"
#include "SDL_keycode.h"
#include "game_math/Vector.hpp"

#include <iostream>

using namespace game_math;

SceneExample::SceneExample(
    const std::shared_ptr<SDLWindow>& window,
    const std::shared_ptr<InputProcessor>& inputProcessor)
    : Scene(Vector2F {
          static_cast<float>(window->getWidth()),
          static_cast<float>(window->getHeight()) })
{
    rootObject = std::make_shared<GameObject<2, float>>();

    mario = std::make_shared<PhysicsGameObject2F>(
        Vector2F { 50, 50 }, Vector2F { cellSize, cellSize }, 1);

    mario->setName("mario");

    mario->setTextureFromFile(
        "/home/user/projects/sdl_example/mario.bmp",
        window->getSdlRenderer());

    marioChild = std::make_shared<PhysicsGameObject2F>(
        Vector2F { 100, 50 }, Vector2F { cellSize, cellSize }, 1);

    marioChild->setName("mario child");

    marioChild->setTextureFromFile(
        "/home/user/projects/sdl_example/mario.jpg",
        window->getSdlRenderer());

    mario->addChild(marioChild);

    rootObject->addChild(mario);
    // setRootObject(rootObject);

    // mario->setSpeed({ 0, -9.8f });

    inputProcessor->subscribeOnKey(
        Key::W,
        [this]() {
            mario->addForce({ 0, moveCoefficient });
        },
        [this]() {
            mario->addForce({ 0, -moveCoefficient });
        });

    inputProcessor->subscribeOnKey(
        Key::S,
        [this]() {
            mario->addForce({ 0, -moveCoefficient });
        },
        [this]() {
            mario->addForce({ 0, moveCoefficient });
        });

    inputProcessor->subscribeOnKey(
        Key::D,
        [this]() {
            mario->addForce({ moveCoefficient, 0 });
        },
        [this]() {
            mario->addForce({ -moveCoefficient, 0 });
        });

    inputProcessor->subscribeOnKey(
        Key::A,
        [this]() {
            mario->addForce({ -moveCoefficient, 0 });
        },
        [this]() {
            mario->addForce({ moveCoefficient, 0 });
        });
}

void SceneExample::processFrame(int32_t)
{
    static int i = 0;

    size_t currentTime = Core::getCore()->getTimeMicroseconds();

    if (lastObjectCreationgTime + objectCreatingPeriod < currentTime) {
        for (const std::shared_ptr<MovableGameObject2F>& gameObject : objects) {
            /*auto& position = gameObject->getPositionRef();

            if (position[1] > 0) {
                position[1] -= cellSize;
            }
            if (position[1] < 0) {
                position[1] = 0;
            }*/
        }

        if (i == 2)
            return;
        else {
            ++i;

            float randomX = rand() % (int(getSize()[0]) / int(cellSize) + 1) * cellSize;
            auto object = std::make_shared<MovableGameObject2F>(
                Vector2F { randomX, getSize()[1] }, Vector2F { cellSize, cellSize });

            object->setColor(255, 0, 0);
            rootObject->addChild(object);

            objects.push_back(object);
        }

        lastObjectCreationgTime = currentTime;
    }
}

void SceneExample::setCollisionsProcessor(
    const std::shared_ptr<CollisionsProcessor<2, float>>& collisionsProcessor)
{
    collisionsProcessor->subscribeOnCollision(
        mario,
        [](const auto& obj) {
            std::cout
                << "Mario started colliding with "
                << obj
                << std::endl;
        },
        [](const auto& obj) {
            std::cout
                << "Mario finished colliding with "
                << obj
                << std::endl;
        });

    collisionsProcessor->subscribeOnCollision(
        marioChild,
        [](const auto& obj) {
            std::cout
                << "MarioChild started colliding with "
                << obj
                << std::endl;
        },
        [](const auto& obj) {
            std::cout
                << "MarioChild finished colliding with "
                << obj
                << std::endl;
        });

    collisionsProcessor->subscribeOnCollision(
        mario,
        marioChild,
        [] {
            std::cout
                << "Mario and MarioChild started colliding with each other"
                << std::endl;
        },
        [] {
            std::cout
                << "Mario and MarioChild finished colliding with each other"
                << std::endl;
        });
}
