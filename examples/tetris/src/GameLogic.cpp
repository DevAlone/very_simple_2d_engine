#include "GameLogic.h"
#include "Exception.h"
#include "MovableGameObject.hpp"
#include "Tetromino.h"
#include "TimersProcessor.h"

using namespace game_math;
using namespace globals;

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

    fallingTimer = timersProcessor->createTimer(1000 * 1000 * 1);
    fallingTimer->addHandler([this] {
        this->handleTimer();
    });
}

void GameLogic::onModuleCreation()
{
}

void GameLogic::processFrame(int32_t)
{
}

void GameLogic::tryToMoveCurrentTetrominoRight()
{
    if (currentTetromino && canTetrominoBeMoved(currentTetromino, { 1, 0 })) {
        currentTetromino->move({ blockSize, 0 });
    }
}

void GameLogic::tryToMoveCurrentTetrominoLeft()
{
    if (currentTetromino && canTetrominoBeMoved(currentTetromino, { -1, 0 })) {
        currentTetromino->move({ -blockSize, 0 });
    }
}

void GameLogic::moveCurrentTetrominoDown()
{
    if (currentTetromino) {
        while (canTetrominoBeMoved(currentTetromino, { 0, -1 })) {
            currentTetromino->move({ 0, -blockSize });
        }
        currentTetromino = nullptr;
    }
}

void GameLogic::rotateCurrentTetrominoClockwise()
{
    if (currentTetromino) {
        if (canTetrominoBeRotatedClockwise(currentTetromino)) {
            currentTetromino->rotateClockwise();
        }
    }
}

void GameLogic::handleTimer()
{
    if (currentTetromino) {
        if (canTetrominoBeMoved(currentTetromino, { 0, -1 })) {
            currentTetromino->move({ 0, -blockSize });
        } else {
            currentTetromino.reset();
        }
    } else {
        deleteLines();

        BaseType x = (rand() % (sceneColumns - BaseType(blocksPerTetromino))) * blockSize;
        BaseType y = this->scene->getSize()[1];

        currentTetromino = Tetromino::createRandom({ x, y }, blockSize);

        this->scene->getRootObject()->addChild(currentTetromino);
    }

    for (size_t row = 0; row < sceneRows; ++row) {
        for (size_t column = 0; column < sceneColumns; ++column) {
            std::cout << this->sceneMap->getData()[row][column].size() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    /*
    fallingTimerPeriod = size_t(float(fallingTimerPeriod) * fallingPeriodChangingCoefficient);
    if (fallingTimerPeriod < minimumFallingPeriod) {
        fallingTimerPeriod = minimumFallingPeriod;
    }
    fallingTimer->setInterval(fallingTimerPeriod);
    */
}

bool GameLogic::canTetrominoBeMoved(
    const std::shared_ptr<Tetromino>& tetromino,
    const game_math::Vector<2, BaseType>& direction) const
{
    for (auto child : tetromino->getChildren()) {
        if (auto movableChild
            = std::dynamic_pointer_cast<MovableGameObject<nDimensions, BaseType>>(child)) {

            auto position = movableChild->getPosition();
            game_math::Vector<2, BaseType> positionInMap = position;
            positionInMap[0] /= blockSize;
            positionInMap[1] /= blockSize;

            auto newPositionInMap = positionInMap + direction;

            if ((direction[0] <= BaseType(0) && newPositionInMap[0] < BaseType(0))
                || (direction[0] >= BaseType(0) && newPositionInMap[0] >= sceneColumns)) {
                return false;
            }
            if ((direction[1] <= BaseType(0) && newPositionInMap[1] < BaseType(0))
                || (direction[1] >= BaseType(0) && newPositionInMap[1] >= sceneRows)) {
                return false;
            }

            auto& cell = this->sceneMap->getData()
                             [static_cast<size_t>(newPositionInMap[1])]
                             [static_cast<size_t>(newPositionInMap[0])];

            for (const auto& item : cell) {
                if (item->getParent() != tetromino.get()) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool GameLogic::canTetrominoBeRotatedClockwise(std::shared_ptr<Tetromino>& tetromino) const
{
    tetromino->rotateClockwise();
    for (size_t row = 0; row < sceneRows; ++row) {
        for (size_t column = 0; column < sceneColumns; ++column) {
            std::cout << this->sceneMap->getData()[row][column].size() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    bool result = canTetrominoBeMoved(tetromino, { 0, 0 });
    tetromino->rotateCounterclockwise();

    return result;
}

void GameLogic::deleteLines()
{
    // TODO: fix bug

    for (size_t nRow = 0; nRow < globals::sceneRows; ++nRow) {
        bool needsToBeRemoved = true;
        for (size_t nColumn = 0; nColumn < globals::sceneColumns; ++nColumn) {
            if (sceneMap->getData()[nRow][nColumn].size() == 0) {
                needsToBeRemoved = false;
                break;
            }
        }

        if (needsToBeRemoved) {
            for (size_t nColumn = 0; nColumn < globals::sceneColumns; ++nColumn) {
                for (auto blockPtr : sceneMap->getData()[nRow][nColumn]) {
                    if (auto tetromino = dynamic_cast<Tetromino*>(blockPtr->getParent())) {
                        tetromino->removeChild(blockPtr);
                    }
                }
            }
        }
    }
}
