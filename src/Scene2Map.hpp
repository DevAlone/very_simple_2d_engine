#pragma once

#include "Module.h"
#include "MovableGameObject.hpp"
#include "Scene.hpp"
#include "game_math/game_math.h"

#include <array>
#include <cassert>
#include <functional>
#include <memory>
#include <unordered_set>

// TODO: fix objects overlaying each other
// TODO: cover with unit tests

/**
 * Scene2Map - map of objects inside 2 dimensional scene
 */
template <typename BaseType, size_t nRows, size_t nColumns>
class Scene2Map : public Module {
    using ScenePtr = std::shared_ptr<Scene<2, BaseType>>;
    using ElementsSet = std::unordered_set<MovableGameObject<2, BaseType>*>;
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

    auto getCellByIndex(size_t nRow, size_t nColumn) const -> ElementsSet;

    // TODO: remove
    /**
     * @brief clean - cleans obsolete values from the map, do not call while iterating
     */
    void clean();

private:
    auto getCellsForRectangle(
        game_math::Vector<2, BaseType> position,
        game_math::Vector<2, BaseType> size) -> std::vector<std::reference_wrapper<ElementsSet>>;

    void handleObjectPositionChanged(
        MovableGameObject<2, BaseType>* object,
        const game_math::Vector<2, BaseType>& previousPosition);

    void handleObjectRemoved(GameObject<2, BaseType>* object);

    const std::shared_ptr<Scene<2, BaseType>> scene;

    std::unordered_map<const MovableGameObject<2, BaseType>*,
        std::vector<std::reference_wrapper<ElementsSet>>>

        elementCellsMap;

    game_math::Matrix<nRows, nColumns, ElementsSet> data;
    /*
     std::vector<std::pair<std::reference_wrapper<ElementsSet>, MovableGameObject<2, BaseType>*>>
        objectsToRemove;
    */

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

    scene->subscribeOnObjectPositionChanged([this](auto object, auto position) {
        handleObjectPositionChanged(object, position);
    });

    scene->subscribeOnObjectRemoved([this](auto object) {
        handleObjectRemoved(object);
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
auto Scene2Map<BaseType, nRows, nColumns>::getCellByIndex(size_t nRow, size_t nColumn) const
    -> ElementsSet
{
    return data[nRow][nColumn];
}

/*template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::clean()
{
    // TODO: syncronize with adding items to vector
    for (const auto& pair : objectsToRemove) {
        ElementsSet& cell = pair.first;
        auto objectPtr = pair.second;
        cell.erase(objectPtr);
    }
    objectsToRemove.clear();
}*/

template <typename BaseType, size_t nRows, size_t nColumns>
auto Scene2Map<BaseType, nRows, nColumns>::getCellsForRectangle(
    game_math::Vector<2, BaseType> position,
    game_math::Vector<2, BaseType> size)
    -> std::vector<std::reference_wrapper<ElementsSet>>
{
    // ignore items which are not in the scene at all
    if (position[0] >= scene->getSize()[0]) {
        return {};
    }
    if (position[1] >= scene->getSize()[1]) {
        return {};
    }
    if (position[0] + size[0] <= BaseType()) {
        return {};
    }
    if (position[1] + size[1] <= BaseType()) {
        return {};
    }

    // normalize values which are in the scene at least partly to not overflow the matrix
    if (position[0] < BaseType()) {
        position[0] = BaseType();
    }
    if (position[1] < BaseType()) {
        position[1] = BaseType();
    }

    if (size[0] < BaseType()) {
        size[0] = BaseType();
    }
    if (size[1] < BaseType()) {
        size[1] = BaseType();
    }

    if (size[0] > scene->getSize()[0]) {
        size[0] = scene->getSize()[0];
    }
    if (size[1] > scene->getSize()[1]) {
        size[1] = scene->getSize()[1];
    }

    if (position[0] + size[0] > scene->getSize()[0]) {
        position[0] = scene->getSize()[0] - size[0];
    }
    if (position[1] + size[1] > scene->getSize()[1]) {
        position[1] = scene->getSize()[1] - size[1];
    }

    size_t startColumn = position[0] / cellSize[0];
    size_t startRow = position[1] / cellSize[1];

    size_t endColumn = (position[0] + size[0]) / cellSize[0];
    size_t endRow = (position[1] + size[1]) / cellSize[1];

    std::vector<std::reference_wrapper<ElementsSet>> result;

    for (size_t nRow = startRow; nRow < endRow; ++nRow) {
        for (size_t nColumn = startColumn; nColumn < endColumn; nColumn++) {
            result.push_back(data[nRow][nColumn]);
        }
    }

    return result;
}

template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::handleObjectPositionChanged(
    MovableGameObject<2, BaseType>* object,
    const game_math::Vector<2, BaseType>&)
{
    assert(object != nullptr);

    {
        // remove from old cells
        auto it = elementCellsMap.find(object);
        if (it != elementCellsMap.end()) {
            for (ElementsSet& cell : it->second) {
                cell.erase(object);
            }
            elementCellsMap.erase(object);
        }
    }

    // insert to new cells
    for (ElementsSet& cell : getCellsForRectangle(object->getPosition(), object->getSize())) {
        cell.insert(object);
        elementCellsMap[object].push_back(cell);
    }
}

template <typename BaseType, size_t nRows, size_t nColumns>
void Scene2Map<BaseType, nRows, nColumns>::handleObjectRemoved(GameObject<2, BaseType>* object)
{
    auto movableObject = dynamic_cast<MovableGameObject<2, BaseType>*>(object);
    if (movableObject == nullptr) {
        return;
    }

    auto it = elementCellsMap.find(movableObject);
    if (it != elementCellsMap.end()) {

        for (ElementsSet& cell : it->second) {
            // objectsToRemove.push_back({ cell, movableObject });
            cell.erase(movableObject);
        }
        elementCellsMap.erase(movableObject);
    }
}
