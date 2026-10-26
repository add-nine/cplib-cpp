#pragma once

#include <concepts>
#include <cstdint>
#include <istream>
#include <ostream>
#include <type_traits>
#include <utility>

template <class T>
concept Arithmetic = std::signed_integral<T> || std::floating_point<T>;

template <Arithmetic T>
using Calc = std::conditional_t<std::floating_point<T>, T, std::conditional_t<(sizeof(T) <= 4), std::int64_t, __int128_t>>;

template <Arithmetic T>
struct Point {
    T x{}, y{};

    constexpr Point() = default;
    constexpr Point(T x, T y) : x(x), y(y) {}
    template <Arithmetic A, Arithmetic B> constexpr Point(const std::pair<A, B>& p) : x(p.first), y(p.second) {}

    constexpr Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    constexpr Point& operator+=(const Point& p) { return *this = *this + p; }
    constexpr Point operator-() const { return Point(-x, -y); }
    constexpr Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    constexpr Point& operator-=(const Point& p) { return *this = *this - p; }
    template <Arithmetic U> constexpr Point operator*(U s) const { return Point(x * s, y * s); }
    template <Arithmetic U> constexpr Point& operator*=(U s) { return *this = *this * s; }
    template <Arithmetic U> constexpr Point operator/(U s) const { return Point(x / s, y / s); }
    template <Arithmetic U> constexpr Point& operator/=(U s) { return *this = *this / s; }
    template <Arithmetic U> friend constexpr Point operator*(U s, const Point& p) { return p * s; }

    constexpr Calc<T> norm2() const {
        return Calc<T>(x) * x + Calc<T>(y) * y;
    }
    constexpr T rot90(bool ccw = true) const {
        return (ccw ? Point(-y, x) : Point(y, -x));
    }

    friend std::istream& operator>>(std::istream& is, Point& p) { return is >> p.x >> p.y; }
    friend std::ostream& operator<<(std::ostream& os, const Point& p) { return os << p.x << ' ' << p.y; }
};

template <Arithmetic T>
constexpr Calc<T> dot(const Point<T>& a, const Point<T>& b) {
    return Calc<T>(a.x) * b.x + Calc<T>(a.y) * b.y;
}
template <Arithmetic T>
constexpr Calc<T> cross(const Point<T>& a, const Point<T>& b) {
    return Calc<T>(a.x) * b.y - Calc<T>(a.y) * b.x;
}
template <Arithmetic T>
constexpr Calc<T> dist2(const Point<T>& a, const Point<T>& b) {
    return dot(a - b, a - b);
}

struct argless {
    template <Arithmetic T>
    constexpr bool operator()(const Point<T>& a, const Point<T>& b) const {
        auto isZero = [](const Point<T>& p) { return p.x == 0 && p.y == 0; };
        if (isZero(a) || isZero(b)) {
            return isZero(a) && !isZero(b);
        }
        auto upper = [](const Point<T>& p) { return p.y > 0 || (p.y == 0 && p.x > 0); };
        if (upper(a) != upper(b)) {
            return upper(a);
        }
        return cross(a, b) > 0;
    }
};