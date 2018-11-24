#include "GameLogic.h"
#include "Exception.h"
#include "MovableGameObject.hpp"
#include "Tetromino.h"

using namespace game_math;

GameLogic::GameLogic(
    const std::shared_ptr<TetrisScene>& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("invalid scene");
    }

    auto root = std::make_shared<MovableGameObject<nDimensions, BaseType>>(
        game_math::Vector<nDimensions, BaseType>({ 0, 0 }),
        scene->getSize());

    auto shape = Matrix<4, 4, bool>(
        { { 1, 0, 0, 0 },
            { 1, 0, 1, 0 },
            { 1, 0, 0, 0 },
            { 0, 1, 1, 1 } });

    root->addChild(std::shared_ptr<Tetromino>(new Tetromino(
        { 0, 0 },
        blockSize,
        { 255, 0, 0 },
        shape)));

    scene->setRootObject(root);
}

void GameLogic::onModuleCreation()
{
}
