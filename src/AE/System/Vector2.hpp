#pragma once

namespace ae {

template<typename T>
class Vector2 {
public:
    Vector2() = default;
    Vector2(T x, T y);

    // Vector2(std::initializer_list<T> list) { }

    template<typename U>
    Vector2(const Vector2<U>& vector);

public:
    T x = 0; 
    T y = 0;
};

template<typename T>
Vector2<T> operator -(const Vector2<T>& right);

template<typename T>
const Vector2<T> operator +(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);

template<typename T>
const Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right);

template<typename T>
Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);

template<typename T>
const Vector2<T> operator *(const Vector2<T>& left, T right);

template<typename T>
const Vector2<T> operator *(T left, const Vector2<T>& right);

template<typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right);

template<typename T>
const Vector2<T> operator /(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
const Vector2<T> operator /(const Vector2<T>& left, T right);

template<typename T>
Vector2<T>& operator /=(Vector2<T>& left, T right);

template <typename T>
bool operator ==(const Vector2<T>& left, const Vector2<T>& right);

template <typename T>
bool operator !=(const Vector2<T>& left, const Vector2<T>& right);


#include <AE/System/Vector2.inl>

typedef Vector2<int>   Vector2i;
typedef Vector2<float> Vector2f;

}