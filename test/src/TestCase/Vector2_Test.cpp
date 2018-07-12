#include <memory>
#include <gtest/gtest.h>

#include <AE/System/Vector2.hpp>

// Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);
// const Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);
// Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);
// const Vector2<T> operator *(const Vector2<T>& left, T right);
// const Vector2<T> operator *(T left, const Vector2<T>& right);
// Vector2<T>& operator *=(Vector2<T>& left, T right);
// const Vector2<T> operator /(const Vector2<T>& left, const Vector2<T>& right);
// const Vector2<T> operator /(const Vector2<T>& left, T right);
// Vector2<T>& operator /=(Vector2<T>& left, T right);
// bool operator ==(const Vector2<T>& left, const Vector2<T>& right);
// bool operator !=(const Vector2<T>& left, const Vector2<T>& right);

TEST(Vector2Test, construct)
{
    ae::Vector2i vec;

    ASSERT_TRUE((vec.x == 0) && (vec.y == 0));

    vec = ae::Vector2i(5, 6);

    ASSERT_TRUE((vec.x == 5) && (vec.y == 6));

    ae::Vector2f fvec(vec);    

    ASSERT_TRUE((fvec.x == 5.0) && (fvec.y == 6.0));
}

TEST(Vector2Test, operations)
{
    // ae::Vector2i posVec2(1, 2);
    // ae::Vector2i negVec2(-1, -2);

    // ASSERT_TRUE(-posVec2 == negVec2);

    // ASSERT_EQ(posVec2 + negVec2, ae::Vector2i(0, 0));

    // ae::Vector2i vec10(10, 10);

    // posVec2 += vec10;

    // ASSERT_EQ(posVec2, vec10);
}
















