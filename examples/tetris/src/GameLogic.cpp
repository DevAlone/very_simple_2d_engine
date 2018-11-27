#include "GameLogic.h"
#include "Exception.h"
#include "MovableGameObject.hpp"
#include "Tetromino.h"
#include "TimersProcessor.h"

using namespace game_math;

GameLogic::GameLogic(
    const std::shared_ptr<TetrisScene>& scene,
    const std::shared_ptr<Scene2Map<BaseType, sceneRows, sceneColumns>>& sceneMap,
    const std::shared_ptr<TimersProcessor>& timersProcessor)
    : scene(scene)
    , sceneMap(sceneMap)
    , timersProcessor(timersProcessor)
{
    if (!scene || !timersProcessor || !sceneMap) {
        throw Exception("invalid scene or timers processor or scene map");
    }

    auto root = std::make_shared<MovableGameObject<nDimensions, BaseType>>(
        game_math::Vector<nDimensions, BaseType>({ 0, 0 }),
        scene->getSize());

    // TODO: refactor
    Scene<nDimensions, BaseType>::setRootObject(scene, root);

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

    //scene->setRootObject(root);

    timersProcessor->createTimer(1000 * 1000 * 1)->addHandler([this] {
        for (auto tetromino : tetrominos) {
            for (auto child : tetromino->getChildren()) {
                if (auto movableChild
                    = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {

                    auto position = movableChild->getPosition();
                    game_math::Vector<2, int> positionInMap = position;
                    positionInMap[0] /= blockSize;
                    positionInMap[1] /= blockSize;

                    if (positionInMap[0] >= 0 && positionInMap[1] > 0) {
                        if (this->sceneMap->getData()
                                [static_cast<size_t>(positionInMap[1] - 1)]
                                [static_cast<size_t>(positionInMap[0])]
                                    .size()
                            == 0) {

                            position[1] -= blockSize;
                        }
                    }

                    /*if (position[1] < 0) {
                        position[1] = 0;
                    }*/

                    movableChild->setPosition(position);
                }
            }
        }

        for (size_t row = 0; row < sceneRows; ++row) {
            for (size_t column = 0; column < sceneColumns; ++column) {
                std::cout << this->sceneMap->getData()[row][column].size() << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
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
