#include "Core.h"
#include "Exception.h"
#include "GameLogic.h"
#include "InputProcessor.h"
#include "SDLEventsProcessor.h"
#include "Scene2Map.hpp"
#include "Scene2Renderer.hpp"
#include "TetrisScene.h"
#include "TimersProcessor.h"
#include "globals.h"

#include <iostream>

using namespace globals;

void createGame()
{
    auto core = Core::getCore();

    auto scene = core->addModuleOfType<TetrisScene>(sceneWidth, sceneHeight);
    auto timersProcessor = core->addModuleOfType<TimersProcessor>();
    auto sceneMap = core->addModuleOfType<Scene2Map<BaseType, sceneRows, sceneColumns>>(scene);
    auto gameLogic = core->addModuleOfType<GameLogic>(scene, sceneMap, timersProcessor);
    auto sdlEventsProcessor = core->addModuleOfType<SDLEventsProcessor>();
    auto inputProcessor = core->addModuleOfType<InputProcessor>(sdlEventsProcessor);
    auto sdlWindow = core->addModuleOfType<SDLWindow>(sceneWidth, sceneHeight);
    auto sceneRenderer = core->addModuleOfType<Scene2Renderer<BaseType>>(scene, sdlWindow);

    inputProcessor->subscribeOnKey(Key::A, [gameLogic] {
        gameLogic->tryToMoveCurrentTetrominoLeft();
    });
    inputProcessor->subscribeOnKey(Key::D, [gameLogic] {
        gameLogic->tryToMoveCurrentTetrominoRight();
    });
    inputProcessor->subscribeOnKey(Key::S, [gameLogic] {
        gameLogic->moveCurrentTetrominoDown();
    });
    inputProcessor->subscribeOnKey(Key::W, [gameLogic] {
        gameLogic->rotateCurrentTetrominoClockwise();
    });

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
