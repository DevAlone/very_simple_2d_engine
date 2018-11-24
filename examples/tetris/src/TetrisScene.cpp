#include "TetrisScene.h"
#include "game_math/Vector.hpp"

TetrisScene::TetrisScene(unsigned width, unsigned height)
    : Scene(game_math::Vector<nDimensions, BaseType> { width, height })
{
}
