#include "Core.h"
#include "Exception.h"
#include "GameLogic.h"
#include "Scene2Renderer.hpp"
#include "SceneMap.hpp"
#include "TetrisScene.h"
#include "TimersProcessor.h"
#include "globals.h"

#include <iostream>

void createGame()
{
    auto core = Core::getCore();

    auto scene = core->addModuleOfType<TetrisScene>(sceneWidth, sceneHeight);
    auto timersProcessor = core->addModuleOfType<TimersProcessor>();
    auto sceneMap = core->addModuleOfType<SceneMap<nDimensions, BaseType, 3>>(scene);
    auto gameLogic = core->addModuleOfType<GameLogic>(scene, sceneMap, timersProcessor);
    auto sdlWindow = core->addModuleOfType<SDLWindow>(sceneWidth, sceneHeight);
    auto sceneRenderer = core->addModuleOfType<Scene2Renderer<BaseType>>(scene, sdlWindow);

    core->loop();
}

int main()
{
    try {
        createGame();
    } catch (const Exception& exc) {
        std::cerr << exc << std::endl;
    }
}
