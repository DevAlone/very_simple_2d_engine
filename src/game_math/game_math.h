#include "Vector.hpp"

#include <cmath>
#include <limits>

namespace game_math {

template <typename T>
bool nearlyEqual(T value1, T value2)
{
    return std::fabs(value1 - value2) < std::numeric_limits<T>::epsilon();
}

template <typename Type, std::size_t Size>
bool nearlyEqual(
    const Vector<Type, Size>& v1,
    const Vector<Type, Size>& v2)
{
    for (size_t i = 0; i < Size; ++i) {
        if (!nearlyEqual(v1.getData()[i], v2.getData()[i])) {
            return false;
        }
    }

    return true;
}

}
