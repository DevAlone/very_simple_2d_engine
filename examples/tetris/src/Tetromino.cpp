#include "Tetromino.h"
#include "Scene.hpp"

#include <random>
using namespace globals;

Tetromino::Tetromino(
    const game_math::Vector<2, BaseType>& position,
    BaseType pieceSize,
    const game_math::Vector<3, unsigned char>& color,
    const game_math::Matrix<blocksPerTetromino, blocksPerTetromino, bool>& shape)

    : MovableGameObject<2, BaseType>(
          position,
          { 0, 0 })
    , shape(shape)
    , pieceSize(pieceSize)
{
    for (size_t nRow = 0; nRow < blocksPerTetromino; ++nRow) {
        for (size_t nColumn = 0; nColumn < blocksPerTetromino; ++nColumn) {
            if (shape[nRow][nColumn]) {
                auto childSize = game_math::Vector<2, BaseType> { pieceSize, pieceSize };
                auto child = std::make_shared<MovableGameObject<2, BaseType>>(position, childSize);
                child->setColor(color[0], color[1], color[2]);
                tetroChildren[nRow][nColumn] = child;
                addChild(child);
            }
        }
    }
    initChildrenPositions();
}

std::shared_ptr<Tetromino> Tetromino::createRandom(
    const game_math::Vector<2, BaseType>& position,
    BaseType pieceSize)
{
    static std::mt19937 randomGenerator;
    static std::uniform_int_distribution<std::mt19937::result_type> randomDistribution(
        0, tetrominoShapes.size() - 1);

    auto [color, shape] = tetrominoShapes[randomDistribution(randomGenerator)];

    return std::make_shared<Tetromino>(position, pieceSize, color, shape);
}

/*std::shared_ptr<Tetromino> Tetromino::createCopy(const std::shared_ptr<Tetromino>& obj)
{
    if (!obj) {
        return std::shared_ptr<Tetromino>();
    }

    return std::make_shared<Tetromino>(obj->position, obj->pieceSize, obj->color, obj->shape);
}*/

void Tetromino::rotateClockwise()
{
    shape.rotateClockwise();
    tetroChildren.rotateClockwise();
    initChildrenPositions();
}

void Tetromino::rotateCounterclockwise()
{
    shape.rotateCounterclockwise();
    tetroChildren.rotateCounterclockwise();
    initChildrenPositions();
}

void Tetromino::removeChild(GameObject* child)
{
    MovableGameObject::removeChild(child);
    if (child) {
        for (size_t nRow = 0; nRow < blocksPerTetromino; ++nRow) {
            for (size_t nColumn = 0; nColumn < blocksPerTetromino; ++nColumn) {
                auto& tetroChild = tetroChildren[nRow][nColumn];
                if (tetroChild.get() == child) {
                    tetroChild.reset();
                    shape[nRow][nColumn] = 0;
                }
            }
        }
    }

    // remove empty objects from the scene
    bool isEmpty = true;

    for (size_t nRow = 0; nRow < blocksPerTetromino; ++nRow) {
        for (size_t nColumn = 0; nColumn < blocksPerTetromino; ++nColumn) {
            if (shape[nRow][nColumn]) {
                isEmpty = false;
                break;
            }
        }
    }

    if (isEmpty) {
        if (getParent()) {
            getParent()->removeChild(this);
        }
    }
}

void Tetromino::initChildrenPositions()
{
    for (size_t nRow = 0; nRow < blocksPerTetromino; ++nRow) {
        for (size_t nColumn = 0; nColumn < blocksPerTetromino; ++nColumn) {
            if (auto child = tetroChildren[nRow][nColumn]) {
                auto childPosition = game_math::Vector<2, BaseType> {
                    getPosition()[0] + BaseType(nColumn) * pieceSize,
                    getPosition()[1] + getSize()[1] - BaseType(nRow) * pieceSize - pieceSize
                };
                child->setPosition(childPosition);
            }
        }
    }
}
