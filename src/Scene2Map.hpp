#pragma once

#include "Module.h"
#include "MovableGameObject.hpp"
#include "Scene.hpp"
#include "game_math/game_math.h"

#include <array>
#include <memory>
#include <unordered_set>

/**
 * Scene2Map - map of objects inside 2 dimensional scene
 */
template <typename BaseType, size_t nRows, size_t nColumns>
class Scene2Map : public Module {
    using ScenePtr = std::shared_ptr<Scene<2, BaseType>>;
    using ElementsSet = std::unordered_set<const MovableGameObject<2, BaseType>*>;
    // using GameObjectPtr = std::shared_ptr<MovableGameObject<2, BaseType>>;

public:
    /**
     * @brief Scene2Map
     * @param scene
     * @param cells - cells per dimension
     */
    Scene2Map(const ScenePtr& scene);

    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;

    auto getData() const -> const game_math::Matrix<nRows, nColumns, ElementsSet>&;

private:
    void handleObjectPositionChanged(
        const MovableGameObject<2, BaseType>* object,
        const game_math::Vector<2, BaseType>& previousPosition);

    const std::shared_ptr<Scene<2, BaseType>> scene;
    game_math::Matrix<nRows, nColumns, ElementsSet> data;
    game_math::Vector<2, BaseType> cellSize;
};

template <typename BaseType, size_t nRows, size_t nColumns>
Scene2Map<BaseType, nRows, nColumns>::Scene2Map(const ScenePtr& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("bad scene");
    }

    cellSize = scene->getSize();
    cellSize[0] /= nColumns;
    cellSize[1] /= nRows;

    scene->subscribeOnObjectPositionChanged(
        [this](const MovableGameObject<2, BaseType>* object,
            const game_math::Vector<2, BaseType>& position) {
            handleObjectPositionChanged(object, position);
        });
}

template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::onModuleCreation()
{
}

template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::processFrame(int32_t)
{
}

template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::handleObjectPositionChanged(
    const MovableGameObject<2, BaseType>* object,
    const game_math::Vector<2, BaseType>& previousPosition)
{
    // TODO: add debug check on object ptr
    std::cout
        << "object "
        << *object
        << " position changed, previous position"
        << previousPosition
        << std::endl;

    if (previousPosition[0] >= BaseType() && previousPosition[0] <= scene->getSize()[0]
        && previousPosition[1] >= BaseType() && previousPosition[1] <= scene->getSize()[1]) {

        auto cellPreviousIndex = previousPosition;
        cellPreviousIndex[0] /= cellSize[0];
        cellPreviousIndex[1] /= cellSize[1];

        auto& previousCell = data[cellPreviousIndex[1]][cellPreviousIndex[0]];
        previousCell.erase(object);
    }

    if (object->getPosition()[0] >= BaseType() && object->getPosition()[0] <= scene->getSize()[0]
        && object->getPosition()[1] >= BaseType() && object->getPosition()[1] <= scene->getSize()[1]) {

        auto cellPreviousIndex = previousPosition;
        auto cellIndex = object->getPosition();
        cellIndex[0] /= cellSize[0];
        cellIndex[1] /= cellSize[1];

        auto& cell = data[cellIndex[1]][cellIndex[0]];
        cell.insert(object);
    }
}

template <typename BaseType, size_t nRows, size_t nColumns>
auto Scene2Map<BaseType, nRows, nColumns>::getData() const
    -> const game_math::Matrix<nRows, nColumns, ElementsSet>&
{
    return data;
}
