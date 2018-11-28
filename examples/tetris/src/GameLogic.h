#pragma once

class Tetromino;
class TimersProcessor;
class Timer;

#include "Module.h"
#include "Scene2Map.hpp"
#include "TetrisScene.h"

#include <memory>
#include <unordered_set>

class GameLogic : public Module {
public:
    GameLogic(
        const std::shared_ptr<TetrisScene>& scene,
        const std::shared_ptr<Scene2Map<
            globals::BaseType, globals::sceneRows, globals::sceneColumns>>& sceneMap,
        const std::shared_ptr<TimersProcessor>& timersProcessor);

    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;
    void tryToMoveCurrentTetrominoRight();
    void tryToMoveCurrentTetrominoLeft();
    void moveCurrentTetrominoDown();
    void rotateCurrentTetrominoClockwise();

private:
    void handleTimer();

    bool canTetrominoBeMoved(
        const std::shared_ptr<Tetromino>& tetromino,
        const game_math::Vector<2, globals::BaseType>& direction) const;

    bool canTetrominoBeRotatedClockwise(std::shared_ptr<Tetromino>& tetromino) const;

    void deleteLines();

    std::shared_ptr<TetrisScene> scene;

    std::shared_ptr<
        Scene2Map<globals::BaseType, globals::sceneRows, globals::sceneColumns>>
        sceneMap;

    std::shared_ptr<TimersProcessor> timersProcessor;
    std::shared_ptr<Tetromino> currentTetromino;
    size_t fallingTimerPeriod = globals::maximumFallingPeriod;
    std::shared_ptr<Timer> fallingTimer;
};
