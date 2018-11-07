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

        /*eventsProcessor->subscribeOnEvent<KeyEvent>([](const KeyEvent& event) {
            std::cout
                << "Key event happened: "
                << int(event.key)
                << ", "
                << bool(event.keyState)
                << std::endl;
        });*/

        auto inputProcessor = std::dynamic_pointer_cast<InputProcessor>(
            core->addModuleOfType<InputProcessor>(eventsProcessor));

        inputProcessor->subscribeOnKey(
            Key::W,
            [](const KeyEvent&) {
                std::cout << "W pressed" << std::endl;
            },
            [](const KeyEvent&) {
                std::cout << "W is held" << std::endl;
            },
            [](const KeyEvent&) {
                std::cout << "W released" << std::endl;
            });

        inputProcessor->subscribeOnKey(
            Key::W,
            [](const auto&) {
                std::cout << "W pressed 2" << std::endl;
            },
            [](const auto&) {
                std::cout << "W released 2" << std::endl;
            });

        auto scene = std::dynamic_pointer_cast<Scene>(
            core->addModuleOfType<SceneExample>(window));

        core->addModuleOfType<PhysicsProcessor>(scene);

        core->addModuleOfType<SceneRenderer>(scene, window);

        core->loop();
    } catch (const Exception& exception) {
        std::cerr << exception.getMessage() << std::endl;
    }
}
