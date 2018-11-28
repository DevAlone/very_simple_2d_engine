#pragma once

#include "Scene.hpp"
#include "globals.h"

class TetrisScene : public Scene<globals::nDimensions, globals::BaseType> {
public:
    TetrisScene(unsigned width, unsigned height);
};
