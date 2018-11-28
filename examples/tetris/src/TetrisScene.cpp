#include "TetrisScene.h"
#include "game_math/Vector.hpp"

using namespace globals;

TetrisScene::TetrisScene(unsigned width, unsigned height)
    : Scene(game_math::Vector<nDimensions, BaseType> {
          BaseType(width),
          BaseType(height) })
{
}
