#include "Tetromino.h"

Tetromino::Tetromino(
    game_math::Vector<2, BaseType> position,
    BaseType pieceSize,
    game_math::Vector<3, unsigned char> color,
    const game_math::Matrix<4, 4, bool>& shape)

    : MovableGameObject<2, BaseType>(position, { pieceSize * 4, pieceSize * 4 })
    , shape(shape)
{
    for (BaseType nRow = 0; nRow < 4; ++nRow) {
        for (BaseType nColumn = 0; nColumn < 4; ++nColumn) {
            if (shape[nRow][nColumn]) {
                auto childPosition = game_math::Vector<2, BaseType> {
                    position[0] + nColumn * pieceSize,
                    position[1] + size[1] - nRow * pieceSize - pieceSize
                };
                auto childSize = game_math::Vector<2, BaseType> { pieceSize, pieceSize };

                auto child = std::make_shared<MovableGameObject<2, BaseType>>(
                    childPosition, childSize);

                child->setColor(color[0], color[1], color[2]);

                tetroChildren[nRow][nColumn] = child;
                addChild(child);
            }
        }
    }
}
