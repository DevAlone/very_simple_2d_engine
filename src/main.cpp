#include "Core.h"
#include "Exception.h"
#include "InputProcessor.h"
#include "PhysicsProcessor.h"
#include "SDLEventsProcessor.h"
#include "SDLWindow.h"
#include "Scene.h"
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

        auto scene = std::dynamic_pointer_cast<Scene>(
            core->addModuleOfType<SceneExample>(
                window, inputProcessor));

        core->addModuleOfType<PhysicsProcessor>(scene);

        core->addModuleOfType<SceneRenderer>(scene, window);

        core->loop();
    } catch (const Exception& exception) {
        std::cerr << exception.getMessage() << std::endl;
    }
}
