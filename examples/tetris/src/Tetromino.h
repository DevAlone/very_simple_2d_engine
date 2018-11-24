#pragma once

#include "MovableGameObject.hpp"
#include "game_math/game_math.h"
#include "globals.h"

class Tetromino : public MovableGameObject<2, BaseType> {
public:
    Tetromino(
        game_math::Vector<2, BaseType> position,
        BaseType pieceSize,
        game_math::Vector<3, unsigned char> color,
        const game_math::Matrix<4, 4, bool>& shape);

private:
    game_math::Matrix<4, 4, bool> shape;
    game_math::Matrix<4, 4, std::shared_ptr<MovableGameObject<2, BaseType>>> tetroChildren;
};
