#pragma once

#include "Module.h"
#include "MovableGameObject.hpp"
#include "Scene.hpp"
#include "game_math/game_math.h"

#include <array>
#include <memory>
#include <unordered_set>

/**
 * SceneMap - map of objects inside scene
 */
template <std::size_t nDimensions, typename BaseType, std::size_t cells>
class SceneMap : public Module {
    using ScenePtr = std::shared_ptr<Scene<nDimensions, BaseType>>;
    using ElementsSet = std::unordered_set<std::shared_ptr<MovableGameObject<nDimensions, BaseType>>>;
    // using GameObjectPtr = std::shared_ptr<MovableGameObject<nDimensions, BaseType>>;

public:
    /**
     * @brief SceneMap
     * @param scene
     * @param cells - cells per dimension
     */
    SceneMap(const ScenePtr& scene);

    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;

private:
    void handleObjectPositionChanged(
        const MovableGameObject<nDimensions, BaseType>* object,
        const game_math::Vector<nDimensions, BaseType>& previousPosition);

    const std::shared_ptr<Scene<nDimensions, BaseType>> scene;
    std::array<std::array<ElementsSet, cells>, nDimensions> data;
};

template <std::size_t nDimensions, typename BaseType, std::size_t cells>
SceneMap<nDimensions, BaseType, cells>::SceneMap(const ScenePtr& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("bad scene");
    }

    scene->subscribeOnObjectPositionChanged(
        [this](const MovableGameObject<nDimensions, BaseType>* object,
            const game_math::Vector<nDimensions, BaseType>& position) {
            handleObjectPositionChanged(object, position);
        });
}

template <std::size_t nDimensions, typename BaseType, std::size_t cells>
void SceneMap<nDimensions, BaseType, cells>::onModuleCreation()
{
}

template <std::size_t nDimensions, typename BaseType, std::size_t cells>
void SceneMap<nDimensions, BaseType, cells>::processFrame(int32_t)
{
}

template <std::size_t nDimensions, typename BaseType, std::size_t cells>
void SceneMap<nDimensions, BaseType, cells>::handleObjectPositionChanged(
    const MovableGameObject<nDimensions, BaseType>* object,
    const game_math::Vector<nDimensions, BaseType>& previousPosition)
{
    std::cout
        << "object "
        << object
        << " position changed, previous position"
        << previousPosition
        << std::endl;
}
