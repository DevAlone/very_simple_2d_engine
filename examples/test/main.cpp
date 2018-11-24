#include "CollisionsProcessor.hpp"
#include "Core.h"
#include "Exception.h"
#include "InputProcessor.h"
#include "PhysicsProcessor.hpp"
#include "SDLEventsProcessor.h"
#include "SDLWindow.h"
#include "Scene2Renderer.hpp"
#include "SceneExample.h"

#include "SDL2/SDL.h"
#include <iostream>
#include <memory>

const std::size_t worldDimensions = 2;
using WorldBaseType = float;

int main()
{
    try {
        auto core = Core::getCore();
        auto window = std::dynamic_pointer_cast<SDLWindow>(
            core->addModuleOfType<SDLWindow>(640, 480));

        auto eventsProcessor = std::dynamic_pointer_cast<EventsProcessor>(
            core->addModuleOfType<SDLEventsProcessor>());

        auto inputProcessor = std::dynamic_pointer_cast<InputProcessor>(
            core->addModuleOfType<InputProcessor>(eventsProcessor));

        auto scene = std::dynamic_pointer_cast<SceneExample>(
            core->addModuleOfType<SceneExample>(
                window, inputProcessor));

        auto collisionsProcessor
            = std::dynamic_pointer_cast<CollisionsProcessor<worldDimensions, WorldBaseType>>(
                core->addModuleOfType<CollisionsProcessor<worldDimensions, WorldBaseType>>(scene));

        scene->setCollisionsProcessor(collisionsProcessor);

        auto physicsProcessor
            = std::dynamic_pointer_cast<PhysicsProcessor<worldDimensions, WorldBaseType>>(
                core->addModuleOfType<PhysicsProcessor<worldDimensions, WorldBaseType>>(scene));

        // physicsProcessor->addAccelerationForAllObjects({ 0, -9.86f });
        physicsProcessor->addAccelerationForAllObjects({ 0, -300.0f });

        core->addModuleOfType<Scene2Renderer<WorldBaseType>>(scene, window);

        core->loop();
    } catch (const Exception& exception) {
        std::cerr << exception.getMessage() << std::endl;
    }
}
