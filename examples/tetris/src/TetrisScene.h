#pragma once

#include "Scene.hpp"
#include "globals.h"

class TetrisScene : public Scene<nDimensions, BaseType> {
public:
    TetrisScene(unsigned width, unsigned height);
};
