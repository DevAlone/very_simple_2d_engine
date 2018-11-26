#pragma once

class Tetromino;
class TimersProcessor;

#include "Module.h"
#include "SceneMap.hpp"
#include "TetrisScene.h"

#include <memory>
#include <unordered_set>

class GameLogic : public Module {
public:
    GameLogic(
        const std::shared_ptr<TetrisScene>& scene,
        const std::shared_ptr<SceneMap<nDimensions, BaseType, 3>>& sceneMap,
        const std::shared_ptr<TimersProcessor>& timersProcessor);

    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;

private:
    std::shared_ptr<TetrisScene> scene;
    std::shared_ptr<TimersProcessor> timersProcessor;
    std::unordered_set<std::shared_ptr<Tetromino>> tetrominos;
};
