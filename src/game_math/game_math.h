#include "Matrix.hpp"
#include "Vector.hpp"

#include <cmath>
#include <limits>

namespace game_math {

template <typename T>
bool nearlyEqual(T value1, T value2)
{
    return std::fabs(value1 - value2) < std::numeric_limits<T>::epsilon();
}

template <std::size_t Size, typename Type>
bool nearlyEqual(
    const Vector<Size, Type>& v1,
    const Vector<Size, Type>& v2)
{
    for (size_t i = 0; i < Size; ++i) {
        if (!nearlyEqual(v1.getData()[i], v2.getData()[i])) {
            return false;
        }
    }

    return true;
}

}
