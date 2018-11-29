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
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!scene || !timersProcessor || !sceneMap) {
        throw Exception("invalid scene or timers processor or scene map");
    }

    auto root = std::make_shared<MovableGameObject<nDimensions, BaseType>>(
        game_math::Vector<nDimensions, BaseType>({ 0, 0 }),
        scene->getSize());

    root->setColor(150, 150, 150);

    Scene<nDimensions, BaseType>::setRootObject(scene, root);

    fallingTimer = timersProcessor->createTimer(fallingTimerPeriod);
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
        currentTetromino->moveWithChildren({ blockSize, 0 });
    }
}

void GameLogic::tryToMoveCurrentTetrominoLeft()
{
    if (currentTetromino && canTetrominoBeMoved(currentTetromino, { -1, 0 })) {
        currentTetromino->moveWithChildren({ -blockSize, 0 });
    }
}

void GameLogic::moveCurrentTetrominoDown()
{
    if (currentTetromino) {
        while (canTetrominoBeMoved(currentTetromino, { 0, -1 })) {
            currentTetromino->moveWithChildren({ 0, -blockSize });
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
    if (gameEnded) {
        scene->getRootObject()->setColor(255, 150, 150);
        return;
    }

    if (currentTetromino) {
        if (canTetrominoBeMoved(currentTetromino, { 0, -1 })) {
            currentTetromino->moveWithChildren({ 0, -blockSize });
        } else {
            currentTetromino.reset();
        }
    } else {
        if (tryToDeleteLines()) {
            // increase falling speed
            fallingTimerPeriod = size_t(float(fallingTimerPeriod) * fallingPeriodChangingCoefficient);
            if (fallingTimerPeriod < minimumFallingPeriod) {
                fallingTimerPeriod = minimumFallingPeriod;
            }
            fallingTimer->setInterval(fallingTimerPeriod);
        }

        BaseType x = (rand() % (sceneColumns - BaseType(blocksPerTetromino))) * blockSize;
        BaseType y = this->scene->getSize()[1]; //  - blockSize * BaseType(blocksPerTetromino);

        auto tetromino = Tetromino::createRandom({ x, y }, blockSize);

        if (!canTetrominoBeMoved(tetromino, { 0, 0 })) {
            gameEnded = true;
        } else {
            this->scene->getRootObject()->addChild(currentTetromino = tetromino);
        }
    }
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

            auto cell = this->sceneMap->getCellByIndex(
                static_cast<size_t>(newPositionInMap[1]),
                static_cast<size_t>(newPositionInMap[0]));

            for (auto item : cell) {
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

    bool result = canTetrominoBeMoved(tetromino, { 0, 0 });
    tetromino->rotateCounterclockwise();

    return result;
}

bool GameLogic::tryToDeleteLines()
{
    bool wasDeleted = false;

    for (size_t nRow = 0; nRow < sceneRows; ++nRow) {
        bool needsToBeRemoved = true;
        for (size_t nColumn = 0; nColumn < sceneColumns; ++nColumn) {
            if (sceneMap->getCellByIndex(nRow, nColumn).size() == 0) {
                needsToBeRemoved = false;
                break;
            }
        }

        if (needsToBeRemoved) {
            wasDeleted = true;
            for (size_t nColumn = 0; nColumn < sceneColumns; ++nColumn) {
                auto cell = sceneMap->getCellByIndex(nRow, nColumn);
                for (MovableGameObject<nDimensions, BaseType>* blockPtr : cell) {
                    if (auto tetromino = dynamic_cast<Tetromino*>(blockPtr->getParent())) {
                        tetromino->removeChild(blockPtr);
                    }
                }
            }
            moveLinesDown(nRow + 1);
            --nRow;
        }
    }

    return wasDeleted;
}

void GameLogic::moveLinesDown(size_t startLineNumber)
{
    for (size_t nRow = startLineNumber; nRow < sceneRows; ++nRow) {
        for (size_t nColumn = 0; nColumn < sceneColumns; ++nColumn) {
            for (auto blockPtr : sceneMap->getCellByIndex(nRow, nColumn)) {
                if (auto movableBlock
                    = dynamic_cast<MovableGameObject<nDimensions, BaseType>*>(blockPtr)) {

                    if (dynamic_cast<Tetromino*>(blockPtr->getParent())) {
                        movableBlock->moveWithChildren({ 0, -blockSize });
                    }
                }
            }
        }
    }
}
