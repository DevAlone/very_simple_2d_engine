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

        auto scene = std::dynamic_pointer_cast<Scene<2, float>>(
            core->addModuleOfType<SceneExample>(
                window, inputProcessor));

        core->addModuleOfType<PhysicsProcessor<2, float>>(scene);

        core->addModuleOfType<Scene2Renderer<float>>(scene, window);

        core->loop();
    } catch (const Exception& exception) {
        std::cerr << exception.getMessage() << std::endl;
    }
}
