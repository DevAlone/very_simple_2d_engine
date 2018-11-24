#pragma once

#include "Vector.hpp"

#include <array>
#include <cstddef>
#include <iostream>

namespace game_math {

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
class Matrix {
    using RowType = Vector<nColumns, BaseType>;
    using DataType = std::array<RowType, nRows>;

public:
    explicit Matrix(const BaseType& defaultValue);
    Matrix(const DataType& data);
    Matrix(const RowType (&array)[nRows]);
    // Matrix(std::initializer_list<Type> initializerList);

    auto operator[](std::size_t index) -> RowType;
    auto operator==(const Matrix<nRows, nColumns, BaseType>& other) const -> bool;
    auto operator!=(const Matrix<nRows, nColumns, BaseType>& other) const -> bool;

    void transpose();
    void rotateClockwise();
    void rotateCounterclockwise();

    const DataType& getData() const;

private:
    DataType data;
};

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
Matrix<nRows, nColumns, BaseType>::Matrix(const BaseType& defaultValue)
{
    for (size_t i = 0; i < nRows; ++i) {
        data[i] = Vector<nColumns, BaseType>(defaultValue);
    }
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
Matrix<nRows, nColumns, BaseType>::Matrix(const Matrix::DataType& data)
    : data(data)
{
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
Matrix<nRows, nColumns, BaseType>::Matrix(const Matrix::RowType (&array)[nRows])
{
    std::copy(std::begin(array), std::end(array), std::begin(data));
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
auto Matrix<nRows, nColumns, BaseType>::operator[](std::size_t index) -> Vector<nColumns, BaseType>
{
    return data[index];
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
bool Matrix<nRows, nColumns, BaseType>::operator==(const Matrix<nRows, nColumns, BaseType>& other) const
{
    return data == other.data;
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
bool Matrix<nRows, nColumns, BaseType>::operator!=(const Matrix<nRows, nColumns, BaseType>& other) const
{
    return data != other.data;
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
void Matrix<nRows, nColumns, BaseType>::transpose()
{
    static_assert(nRows == nColumns, "number of rows and columns shoud be equal");

    for (size_t row = 0; row < nRows - 1; ++row) {
        for (size_t column = row + 1; column < nColumns; ++column) {
            std::swap(
                data[row][column],
                data[column][row]);
        }
    }
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
void Matrix<nRows, nColumns, BaseType>::rotateClockwise()
{
    // TODO: optimize space efficiency
    static_assert(nRows == nColumns, "number of rows and columns shoud be equal");

    DataType newData;

    for (size_t row = 0; row < nRows; ++row) {
        for (size_t column = 0; column < nColumns; ++column) {
            newData[column][nRows - 1 - row] = data[row][column];
        }
    }

    data = newData;
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
void Matrix<nRows, nColumns, BaseType>::rotateCounterclockwise()
{
    // TODO: optimize space efficiency
    static_assert(nRows == nColumns, "number of rows and columns shoud be equal");

    DataType newData;

    for (size_t row = 0; row < nRows; ++row) {
        for (size_t column = 0; column < nColumns; ++column) {
            newData[nColumns - 1 - column][row] = data[row][column];
        }
    }

    data = newData;
}

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
auto Matrix<nRows, nColumns, BaseType>::getData() const
    -> const std::array<Vector<nColumns, BaseType>, nRows>&
{
    return data;
}

} // end of namespace // TODO: make CMakeLists file to do such comments

template <std::size_t nRows, std::size_t nColumns, typename BaseType>
std::ostream& operator<<(
    std::ostream& stream,
    const game_math::Matrix<nRows, nColumns, BaseType>& matrix)
{
    stream
        << "Matrix { rows: " << nRows
        << ", columns: " << nColumns
        << ", data: Array { ";
    auto& data = matrix.getData();
    for (auto it = data.begin(); it != data.end(); ++it) {
        stream << *it;
        if (it != data.end() - 1) {
            stream << ", ";
        }
    }
    return stream << " }";
}
