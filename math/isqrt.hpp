#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <type_traits>

template <std::integral T>
    requires(not std::same_as<T, bool>)
constexpr T isqrt(T n) {
    assert(n >= 0);
    if (n <= 1) {
        return n;
    }
    T x0 = T{1} << ((std::bit_width(static_cast<std::make_unsigned_t<T>>(n)) + 1) >> 1);
    for (T x1; (x1 = (x0 + n / x0) >> 1) < x0; x0 = x1);
    return x0;
}
