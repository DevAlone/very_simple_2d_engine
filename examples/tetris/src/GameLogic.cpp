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
        this->handleTimer();
    });

    timersProcessor->createTimer(1000 * 1000 * 5)->addHandler([this] {
        auto shape = Matrix<4, 4, bool>(
            { { 1, 0, 0, 0 },
                { 1, 0, 0, 0 },
                { 1, 0, 0, 0 },
                { 1, 0, 0, 0 } });

        BaseType x = 0;
        BaseType y = this->scene->getSize()[1] - blockSize * 4;

        auto tetromino = std::shared_ptr<Tetromino>(new Tetromino(
            { x, y },
            blockSize,
            { 255, 0, 0 },
            shape));
        tetrominos.insert(tetromino);
        this->scene->getRootObject()->addChild(tetromino);
    });
}

void GameLogic::onModuleCreation()
{
}

void GameLogic::processFrame(int32_t)
{
}

void GameLogic::handleTimer()
{
    for (auto tetromino : tetrominos) {
        bool canBeMoved = true;

        for (auto child : tetromino->getChildren()) {
            if (auto movableChild
                = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {

                auto position = movableChild->getPosition();
                game_math::Vector<2, int> positionInMap = position;
                positionInMap[0] /= blockSize;
                positionInMap[1] /= blockSize;

                if (positionInMap[0] < 0 || positionInMap[1] <= 0) {
                    canBeMoved = false;
                    break;
                }

                if (positionInMap[0] >= 0 && positionInMap[1] > 0) {
                    auto& cell = this->sceneMap->getData()
                                     [static_cast<size_t>(positionInMap[1] - 1)]
                                     [static_cast<size_t>(positionInMap[0])];

                    for (const auto& item : cell) {
                        if (item->getParent() != tetromino.get()) {
                            canBeMoved = false;
                            break;
                        }
                    }
                }

                // movableChild->setPosition(position);
            }
        }

        if (canBeMoved) {
            for (auto child : tetromino->getChildren()) {
                if (auto movableChild
                    = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {

                    auto position = movableChild->getPosition();

                    position[1] -= blockSize;

                    movableChild->setPosition(position);
                }
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
}
