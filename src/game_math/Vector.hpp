#pragma once

#include <array>
#include <initializer_list>
#include <iostream>

namespace game_math {

/**
 * @brief class for representing vectors of any size
 * with any base type, NOT initialized by zeros
 */
template <std::size_t Size, typename Type>
class Vector {
public:
    explicit Vector(const Type& defaultValue);
    Vector(const std::array<Type, Size>& array);
    Vector(std::initializer_list<Type> initializerList);

    auto getData() const -> const std::array<Type, Size>&;
    auto getDataReference() -> std::array<Type, Size>&;
    auto setData(const std::array<Type, Size>& value) -> void;
    auto operator[](std::size_t index) -> Type&;
    auto operator[](std::size_t index) const -> const Type&;
    auto operator+(const Vector<Size, Type>& other) const -> Vector<Size, Type>;
    auto operator-(const Vector<Size, Type>& other) const -> Vector<Size, Type>;
    auto operator*(Type value) const -> Vector<Size, Type>;
    auto operator/(Type value) const -> Vector<Size, Type>;
    auto operator+=(const Vector<Size, Type>& other) -> Vector<Size, Type>&;
    auto operator-=(const Vector<Size, Type>& other) -> Vector<Size, Type>&;

protected:
    std::array<Type, Size> data;
};

template <std::size_t Size, typename Type>
Vector<Size, Type>::Vector(const Type& defaultValue)
{
    data.fill(defaultValue);
}

template <std::size_t Size, typename Type>
Vector<Size, Type>::Vector(
    const std::array<Type, Size>& array)
    : data(array)
{
}

template <std::size_t Size, typename Type>
Vector<Size, Type>::Vector(std::initializer_list<Type> initializerList)
{
    std::size_t i = 0;
    for (const auto& item : initializerList) {
        if (i >= Size) {
            break;
        }
        data[i] = item;
        ++i;
    }
}

template <std::size_t Size, typename Type>
std::array<Type, Size>& Vector<Size, Type>::getDataReference()
{
    return data;
}

template <std::size_t Size, typename Type>
const std::array<Type, Size>& Vector<Size, Type>::getData() const
{
    return data;
}

template <std::size_t Size, typename Type>
void Vector<Size, Type>::setData(const std::array<Type, Size>& value)
{
    data = value;
}

template <std::size_t Size, typename Type>
Type& Vector<Size, Type>::operator[](std::size_t index)
{
    return data[index];
}

template <std::size_t Size, typename Type>
const Type& Vector<Size, Type>::operator[](std::size_t index) const
{
    return data[index];
}

template <std::size_t Size, typename Type>
Vector<Size, Type> Vector<Size, Type>::operator+(const Vector<Size, Type>& other) const
{
    Vector<Size, Type> result = *this;
    for (size_t i = 0; i < Size; ++i) {
        result.data[i] += other.data[i];
    }

    return result;
}

template <std::size_t Size, typename Type>
Vector<Size, Type> Vector<Size, Type>::operator-(const Vector<Size, Type>& other) const
{
    Vector<Size, Type> result = *this;
    for (size_t i = 0; i < Size; ++i) {
        result.data[i] -= other.data[i];
    }

    return result;
}

template <std::size_t Size, typename Type>
Vector<Size, Type> Vector<Size, Type>::operator*(Type value) const
{
    Vector<Size, Type> result = *this;
    for (Type& item : result.data) {
        item *= value;
    }
    return result;
}

template <std::size_t Size, typename Type>
Vector<Size, Type> Vector<Size, Type>::operator/(Type value) const
{
    Vector<Size, Type> result = *this;
    for (Type& item : result.data) {
        item /= value;
    }
    return result;
}

template <std::size_t Size, typename Type>
Vector<Size, Type>& Vector<Size, Type>::operator+=(const Vector<Size, Type>& other)
{
    for (size_t i = 0; i < Size; ++i) {
        data[i] += other.data[i];
    }
    return *this;
}

template <std::size_t Size, typename Type>
Vector<Size, Type>& Vector<Size, Type>::operator-=(const Vector<Size, Type>& other)
{
    for (size_t i = 0; i < Size; ++i) {
        data[i] -= other.data[i];
    }
    return *this;
}

template <std::size_t Size, typename Type>
std::ostream& operator<<(std::ostream& stream, const Vector<Size, Type>& vector)
{
    stream << "[";

    for (size_t i = 0; i < Size; ++i) {
        stream << vector.getData()[i];
        if (i != Size - 1)
            stream << ", ";
    }

    return stream << "]";
}

/*template <typename Type>
class Vector2 : public Vector<2, Type> {
public:
    Vector2(Type x = 0, Type y = 0)
    {
        setX(x);
        setY(y);
    }
    Type getX() const { return this->data[0]; }
    Type& getXRef() { return this->data[0]; }
    Type getY() const { return this->data[1]; }
    Type& getYRef() { return this->data[1]; }
    void setX(Type value) { this->data[0] = value; }
    void setY(Type value) { this->data[1] = value; }
};*/

using Vector2D = Vector<2, double>;
using Vector2F = Vector<2, float>;
}
