#include "Tetromino.h"

#include <random>

Tetromino::Tetromino(
    const game_math::Vector<2, BaseType>& position,
    BaseType pieceSize,
    const game_math::Vector<3, unsigned char>& color,
    const game_math::Matrix<4, 4, bool>& shape)

    : MovableGameObject<2, BaseType>(position, { pieceSize * 4, pieceSize * 4 })
    , shape(shape)
{
    for (size_t nRow = 0; nRow < 4; ++nRow) {
        for (size_t nColumn = 0; nColumn < 4; ++nColumn) {
            if (shape[nRow][nColumn]) {
                auto childPosition = game_math::Vector<2, BaseType> {
                    position[0] + BaseType(nColumn) * pieceSize,
                    position[1] + size[1] - BaseType(nRow) * pieceSize - pieceSize
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

std::shared_ptr<Tetromino> Tetromino::createRandom(
    const game_math::Vector<2, BaseType>& position,
    BaseType pieceSize)
{
    std::vector<std::pair<
        game_math::Vector<3, unsigned char>,
        game_math::Matrix<4, 4, bool>>>
        choices = {
            { { 128, 0, 0 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 },
                    { 0, 0, 0, 0 },
                    { 1, 1, 1, 1 },
                }) },
            { { 192, 192, 192 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 1, 1, 1, 1 },
                    { 0, 0, 0, 1 },
                    { 0, 0, 0, 0 },
                }) },
            { { 160, 32, 240 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 1, 1, 1, 1 },
                    { 1, 0, 0, 0 },
                    { 0, 0, 0, 0 },
                }) },
            { { 0, 0, 128 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 0, 1, 1, 0 },
                    { 0, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                }) },
            { { 0, 100, 0 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 0, 1, 1, 0 },
                    { 1, 1, 0, 0 },
                    { 0, 0, 0, 0 },
                }) },
            { { 150, 75, 0 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 1, 1, 1, 0 },
                    { 0, 1, 0, 0 },
                    { 0, 0, 0, 0 },
                }) },
            { { 0, 128, 128 },
                game_math::Matrix<4, 4, bool>({
                    { 0, 0, 0, 0 },
                    { 1, 1, 0, 0 },
                    { 0, 1, 1, 0 },
                    { 0, 0, 0, 0 },
                }) },
        };

    static std::mt19937 randomGenerator;
    static std::uniform_int_distribution<std::mt19937::result_type> randomDistribution(
        0, choices.size() - 1);

    auto [color, shape] = choices[randomDistribution(randomGenerator)];

    auto result
        = std::make_shared<Tetromino>(position, pieceSize, color, shape);

    return result;
}
