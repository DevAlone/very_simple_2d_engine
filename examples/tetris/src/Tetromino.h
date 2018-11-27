#pragma once

#include "MovableGameObject.hpp"
#include "game_math/game_math.h"
#include "globals.h"

class Tetromino : public MovableGameObject<2, BaseType> {
public:
    Tetromino(
        const game_math::Vector<2, BaseType>& position,
        BaseType pieceSize,
        const game_math::Vector<3, unsigned char>& color,
        const game_math::Matrix<4, 4, bool>& shape);

    static auto createRandom(
        const game_math::Vector<2, BaseType>& position,
        BaseType pieceSize) -> std::shared_ptr<Tetromino>;

private:
    game_math::Matrix<4, 4, bool> shape;
    game_math::Matrix<4, 4, std::shared_ptr<MovableGameObject<2, BaseType>>> tetroChildren;
};
