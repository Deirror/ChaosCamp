#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <cassert>

#include "crt.h"
#include "const.h"

CRT_BEGIN

template<int N>
class Vector {
public:
    Vector(); 

    template<typename... Args>
    Vector(Args...);

    float operator[](int idx) const { return m_Data.at(idx); }
    float& operator[](int idx) { return m_Data.at(idx); }

    Vector<N>& operator+=(const Vector<N>&);
    Vector<N>& operator-=(const Vector<N>&);

    Vector<N>& operator*=(float); 
    Vector<N>& operator/=(float);

    float length_squared() const;
    float length() const { return std::sqrt(length_squared()); }

    bool near_zero() const;

    Vector<N> normalized() const;

    template<int M>
    friend Vector<M> operator+(const Vector<M>&, const Vector<M>&);
    template<int M>
    friend Vector<M> operator-(const Vector<M>&, const Vector<M>&);
    template<int M>
    friend Vector<M> operator*(const Vector<M>&, const Vector<M>&);
    template<int M>
    friend Vector<M> operator*(float, const Vector<M>&);
    template<int M>
    friend Vector<M> operator*(const Vector<M>&, float);
    template<int M>
    friend Vector<M> operator/(const Vector<M>&, float); 

    template<int M>
    friend float dot(const Vector<M>&, const Vector<M>&);
private:
    std::array<float, N> m_Data;
};

template<int N>
Vector<N>::Vector() {
    m_Data.fill(0.0f);
}

template<int N>
template<typename... Args>
Vector<N>::Vector(Args... args) : m_Data{static_cast<float>(args)...} {
    static_assert(sizeof...(Args) == N, "Wrong number of arguments for Vector");
}

template<int N>
Vector<N>& Vector<N>::operator+=(const Vector<N>& other) {
    for (int i = 0; i < N; ++i) m_Data[i] += other[i];
    return *this;
}

template<int N>
Vector<N>& Vector<N>::operator-=(const Vector<N>& other) {
    for (int i = 0; i < N; ++i) m_Data[i] -= other[i];
    return *this;
}

template<int N>
Vector<N>& Vector<N>::operator*=(float t) {
    for (int i = 0; i < N; ++i) m_Data[i] *= t;
    return *this;
}

template<int N>
Vector<N>& Vector<N>::operator/=(float t) {
    return *this *= 1.0f / t;
}

template<int N>
float Vector<N>::length_squared() const {
    float sum = 0.0f;
    for (int i = 0; i < N; ++i) sum += m_Data[i] * m_Data[i];
    return sum;
}

template<int N>
bool Vector<N>::near_zero() const {
    for (int i = 0; i < N; ++i)
        if (std::fabs(m_Data[i]) >= math::EPSILON_ZERO) return false;
    return true;
}

template<int N>
Vector<N> operator+(const Vector<N>& lhs, const Vector<N>& rhs) {
    Vector<N> result;
    for (int i = 0; i < N; ++i) result[i] = lhs[i] + rhs[i];
    return result;
}

template<int N>
Vector<N> operator-(const Vector<N>& lhs, const Vector<N>& rhs) {
    Vector<N> result;
    for (int i = 0; i < N; ++i) result[i] = lhs[i] - rhs[i];
    return result;
}

template<int N>
Vector<N> operator*(const Vector<N>& lhs, const Vector<N>& rhs) {
    Vector<N> result;
    for (int i = 0; i < N; ++i) result[i] = lhs[i] * rhs[i];
    return result;
}

template<int N>
Vector<N> operator*(float t, const Vector<N>& other) {
    Vector<N> result;
    for (int i = 0; i < N; ++i) result[i] = other[i] * t;
    return result;
}

template<int N>
Vector<N> operator*(const Vector<N>& other, float t) {
    return t * other;
}

template<int N>
Vector<N> operator/(const Vector<N>& other, float t) {
    return (1.0f / t) * other;
}

template<int N>
float dot(const Vector<N>& lhs, const Vector<N>& rhs) {
    float result = 0.0f;
    for (int i = 0; i < N; ++i) result += lhs[i] * rhs[i];
    return result;
}

template<int N>
Vector<N> Vector<N>::normalized() const {
    return *this / length();
}

using Vec2 = Vector<2>;
using Vec3 = Vector<3>;
using Vec4 = Vector<4>;

CRT_END

