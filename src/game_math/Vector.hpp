#pragma once

#include <array>
#include <iostream>

namespace game_math {

template <typename Type, std::size_t Size>
class Vector {
public:
    Vector() {}

    std::array<Type, Size>& getDataReference();
    const std::array<Type, Size>& getData() const;
    void setData(const std::array<Type, Size>& value);
    Type& operator[](std::size_t index);
    Vector<Type, Size> operator+(const Vector<Type, Size>& other) const;
    Vector<Type, Size> operator-(const Vector<Type, Size>& other) const;

    Vector<Type, Size> operator*(Type value) const;
    Vector<Type, Size> operator/(Type value) const;

    Vector<Type, Size>& operator+=(const Vector<Type, Size>& other);
    Vector<Type, Size>& operator-=(const Vector<Type, Size>& other);

protected:
    std::array<Type, Size> data;
};

template <typename Type, std::size_t Size>
std::array<Type, Size>& Vector<Type, Size>::getDataReference()
{
    return data;
}

template <typename Type, std::size_t Size>
const std::array<Type, Size>& Vector<Type, Size>::getData() const
{
    return data;
}

template <typename Type, std::size_t Size>
void Vector<Type, Size>::setData(const std::array<Type, Size>& value)
{
    data = value;
}

template <typename Type, std::size_t Size>
Type& Vector<Type, Size>::operator[](std::size_t index)
{
    return data[index];
}

template <typename Type, std::size_t Size>
Vector<Type, Size> Vector<Type, Size>::operator+(const Vector<Type, Size>& other) const
{
    Vector<Type, Size> result = *this;
    for (size_t i = 0; i < Size; ++i) {
        result.data[i] += other.data[i];
    }

    return result;
}

template <typename Type, std::size_t Size>
Vector<Type, Size> Vector<Type, Size>::operator-(const Vector<Type, Size>& other) const
{
    Vector<Type, Size> result = *this;
    for (size_t i = 0; i < Size; ++i) {
        result.data[i] -= other.data[i];
    }

    return result;
}

template <typename Type, std::size_t Size>
Vector<Type, Size> Vector<Type, Size>::operator*(Type value) const
{
    Vector<Type, Size> result = *this;
    for (Type& item : result.data) {
        item *= value;
    }
    return result;
}

template <typename Type, std::size_t Size>
Vector<Type, Size> Vector<Type, Size>::operator/(Type value) const
{
    Vector<Type, Size> result = *this;
    for (Type& item : result.data) {
        item /= value;
    }
    return result;
}

template <typename Type, std::size_t Size>
Vector<Type, Size>& Vector<Type, Size>::operator+=(const Vector<Type, Size>& other)
{
    for (size_t i = 0; i < Size; ++i) {
        data[i] += other.data[i];
    }
    return *this;
}

template <typename Type, std::size_t Size>
Vector<Type, Size>& Vector<Type, Size>::operator-=(const Vector<Type, Size>& other)
{
    for (size_t i = 0; i < Size; ++i) {
        data[i] -= other.data[i];
    }
    return *this;
}

template <typename Type, std::size_t Size>
std::ostream& operator<<(std::ostream& stream, const Vector<Type, Size>& vector)
{
    stream << "[";

    for (size_t i = 0; i < Size; ++i) {
        stream << vector.getData()[i];
        if (i != Size - 1)
            stream << ", ";
    }

    return stream << "]";
}

template <typename Type>
class Vector2 : public Vector<Type, 2> {
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
};

using Vector2D = Vector2<double>;
using Vector2F = Vector2<float>;

}
