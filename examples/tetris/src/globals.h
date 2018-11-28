#pragma once

#include "game_math/game_math.h"

#include <cstddef>
#include <vector>

namespace globals {

const size_t nDimensions = 2;
// BaseType should be signed
using BaseType = int;
const BaseType blockSize = 25;
const size_t blocksPerTetromino = 4;
const BaseType sceneColumns = 10;
const BaseType sceneRows = 20;
const BaseType sceneWidth = blockSize * sceneColumns;
const BaseType sceneHeight = blockSize * sceneRows;
// blocks per second
const BaseType tetrominoSpeed = 1;
// in microseconds
const size_t maximumFallingPeriod = 1000 * 1000 * 1;
const size_t minimumFallingPeriod = 1000 * 10;
// falling period is multiplied by this value at the end of level
const float fallingPeriodChangingCoefficient = 0.9f;

static const std::vector<std::pair<
    game_math::Vector<3, unsigned char>,
    game_math::Matrix<4, 4, bool>>>
    tetrominoShapes = {
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

}
