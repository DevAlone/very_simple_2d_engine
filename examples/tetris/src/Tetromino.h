#pragma once

#include "MovableGameObject.hpp"
#include "game_math/game_math.h"
#include "globals.h"

class Tetromino : public MovableGameObject<2, globals::BaseType> {
public:
    Tetromino(
        const game_math::Vector<2, globals::BaseType>& position,
        globals::BaseType pieceSize,
        const game_math::Vector<3, unsigned char>& color,
        const game_math::Matrix<
            globals::blocksPerTetromino, globals::blocksPerTetromino, bool>& shape);

    static auto createRandom(
        const game_math::Vector<2, globals::BaseType>& position,
        globals::BaseType pieceSize) -> std::shared_ptr<Tetromino>;

    /*static auto createCopy(
        const std::shared_ptr<Tetromino>& obj) -> std::shared_ptr<Tetromino>;*/

    void rotateClockwise();
    void rotateCounterclockwise();

    virtual void removeChild(GameObject* child) override;

private:
    void initChildrenPositions();

    game_math::Matrix<globals::blocksPerTetromino, globals::blocksPerTetromino, bool> shape;

    game_math::Matrix<
        globals::blocksPerTetromino,
        globals::blocksPerTetromino,
        std::shared_ptr<MovableGameObject<globals::nDimensions, globals::BaseType>>>
        tetroChildren;

    const globals::BaseType pieceSize;
};
