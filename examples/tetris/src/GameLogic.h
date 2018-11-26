#pragma once

class Tetromino;
class TimersProcessor;

#include "Module.h"
#include "TetrisScene.h"

#include <memory>
#include <unordered_set>

class GameLogic : public Module {
public:
    GameLogic(
        const std::shared_ptr<TetrisScene>& scene,
        const std::shared_ptr<TimersProcessor>& timersProcessor);

    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;

private:
    std::shared_ptr<TetrisScene> scene;
    std::shared_ptr<TimersProcessor> timersProcessor;
    std::unordered_set<std::shared_ptr<Tetromino>> tetrominos;
};
