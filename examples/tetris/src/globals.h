#pragma once

#include <cstddef>

const std::size_t nDimensions = 2;
using BaseType = int;
const BaseType blockSize = 25;
const BaseType sceneColumns = 10;
const BaseType sceneRows = 20;
const BaseType sceneWidth = blockSize * sceneColumns;
const BaseType sceneHeight = blockSize * sceneRows;
// blocks per second
const BaseType tetrominoSpeed = 1;
