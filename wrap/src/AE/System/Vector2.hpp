#pragma once

namespace ae {


void some();

template<typename T>
class Vector2 {
public:
    T x, y;

    //+, -, *, /, +=, -=, *=, /=, ++, --,


};

template<typename T>
Vector2<T> operator ++(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T> operator --(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T> operator ++(const Vector2<T> left, int);

template<typename T>
Vector2<T> operator --(const Vector2<T> left, int);

template<typename T>
Vector2<T> operator +(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T> right);

template<typename T>
Vector2<T> operator -(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T> right);

template<typename T>
Vector2<T> operator *(const Vector2<T> left, T right);

template<typename T>
Vector2<T> operator *(T left, const Vector2<T> right);

template<typename T>
Vector2<T>& operator *=(Vector2<T>& left, T right);

template<typename T>
Vector2<T> operator /(const Vector2<T> left, const Vector<T> right);

template<typename T>
Vector2<T>& operator /=(Vector<T>& left, T right);

template<typename T>
Vector2<T> operator -(const Vector2<T> left, const Vector2<T> right);

template<typename T>
Vector2<T> operator -(const Vector2<T> left, const Vector2<T> right);


}