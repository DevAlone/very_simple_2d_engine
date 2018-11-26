#include "GameLogic.h"
#include "Exception.h"
#include "MovableGameObject.hpp"
#include "Tetromino.h"
#include "TimersProcessor.h"

using namespace game_math;

GameLogic::GameLogic(
    const std::shared_ptr<TetrisScene>& scene,
    const std::shared_ptr<TimersProcessor>& timersProcessor)
    : scene(scene)
    , timersProcessor(timersProcessor)
{
    if (!scene || !timersProcessor) {
        throw Exception("invalid scene or timers processor");
    }

    auto root = std::make_shared<MovableGameObject<nDimensions, BaseType>>(
        game_math::Vector<nDimensions, BaseType>({ 0, 0 }),
        scene->getSize());

    auto shape = Matrix<4, 4, bool>(
        { { 1, 0, 0, 0 },
            { 1, 0, 1, 0 },
            { 1, 0, 0, 0 },
            { 0, 1, 1, 1 } });

    BaseType x = 0;
    BaseType y = scene->getSize()[1] - blockSize * 4;

    auto tetromino = std::shared_ptr<Tetromino>(new Tetromino(
        { x, y },
        blockSize,
        { 255, 0, 0 },
        shape));

    tetrominos.insert(tetromino);
    root->addChild(tetromino);

    scene->setRootObject(root);

    timersProcessor->createTimer(1000 * 1000 * 1)->addHandler([this] {
        for (auto tetromino : tetrominos) {
            for (auto child : tetromino->getChildren()) {
                if (auto movableChild
                    = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {
                    auto& positionRef = movableChild->getPositionRef();

                    positionRef[1] -= blockSize;
                    if (positionRef[1] < 0) {
                        positionRef[1] = 0;
                    }
                }
            }
        }
    });
}

void GameLogic::onModuleCreation()
{
}

void GameLogic::processFrame(int32_t deltaTime)
{
    /*for (std::shared_ptr<Tetromino> tetromino1 : tetrominos) {
        for (std::shared_ptr<GameObject<2, BaseType>> tetromino2 : tetromino1->getChildren()) {
            if (auto tetromino
                = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(tetromino2)) {

                auto& positionRef = tetromino->getPositionRef();

                positionRef[1] -= tetrominoSpeed; // * float(deltaTime) / 1000000 * blockSize;
            }
        }
    }*/
}
