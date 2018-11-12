#pragma once

#include "Module.h"
#include "Scene.hpp"

#include <memory>

template <std::size_t Size, typename Type>
class CollisionsProcessor : public Module {
public:
    CollisionsProcessor(const std::shared_ptr<Scene<Size, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override;

private:
    std::shared_ptr<Scene<Size, Type>> scene;
};

template <std::size_t Size, typename Type>
CollisionsProcessor<Size, Type>::CollisionsProcessor(
    const std::shared_ptr<Scene<Size, Type>>& scene)
    : scene(scene)
{
    if (!scene) {
        throw Exception("bad scene");
    }
}

template <typename Type>
class CollisionsProcessor<2, Type> : public Module {
public:
    CollisionsProcessor(const std::shared_ptr<Scene<2, Type>>& scene);

    virtual void processFrame(int32_t deltaTime) override
    {
    }
};
