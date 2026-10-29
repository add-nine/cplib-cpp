/*
templates and libraries are available at: https://github.com/add-nine/cplib-cpp
*/

#ifndef ADDNINE
#define ADDNINE
#include __FILE__

using namespace std;
using ll = long long;
#define ALL(v) std::begin(v), std::end(v)

int main() {
    
}

#else

#include <bits/stdc++.h>
#include <format>
#include <print>

namespace ad9 {
namespace io {

struct delimiter_t {
    std::string_view element = " ";
    std::string_view nested = "\n";
};

constexpr delimiter_t delim(std::string_view element = " ", std::string_view nested = "\n") noexcept {
    return {element, nested};
}

template<int off, class T>
struct shifted_ref {
    const T& x;
    delimiter_t delims{};
};

template<int off>
struct shifted_t {
    template<class T>
    constexpr shifted_ref<off, T> operator()(const T& x, delimiter_t delims = {}) const noexcept {
        return {x, delims};
    }
};

inline constexpr shifted_t<0> raw{};
inline constexpr shifted_t<+1> inc{};
inline constexpr shifted_t<-1> dec{};

namespace detail {

template<class T>
using raw_t = std::remove_cvref_t<T>;
template<class T> concept StringLike = std::same_as<raw_t<T>, std::string> || std::same_as<raw_t<T>, std::string_view> || std::convertible_to<T, std::string_view>;
template<class T> concept Char = std::same_as<raw_t<T>, char>;
template<class T> concept Range = std::ranges::range<T> && not StringLike<T>;
template<class T> concept TupleLike = not Range<T> && not StringLike<T> && requires { typename std::tuple_size<raw_t<T>>::type; };
template<class T> concept Shiftable = std::integral<raw_t<T>> && not std::same_as<raw_t<T>, bool> && not Char<T>;

template<class T>
using range_value_t = std::ranges::range_value_t<raw_t<T>>;
template<class T> concept NestedRange = Range<T> && (Range<range_value_t<T>> || TupleLike<range_value_t<T>>);

template<int off, class T>
void read_shifted(std::istream& is, T& x) {
    if constexpr (Shiftable<T>) {
        if constexpr (std::same_as<raw_t<T>, signed char> || std::same_as<raw_t<T>, unsigned char>) {
            int v;
            is >> v;
            if (is) x = static_cast<T>(v + off);
        }
        else {
            is >> x; x += off;
        }
    }
    else if constexpr (Range<T>) {
        for (auto& e : x) {
            read_shifted<off>(is, e);
        }
    }
    else if constexpr (TupleLike<T>) {
        std::apply([&](auto&... xs){ (read_shifted<off>(is, xs), ...); }, x);
    }
    else {
        is >> x;
    }
}

template<int off, class T>
void write_shifted(std::ostream& os, const T& x, delimiter_t delims = {}) {
    if constexpr (Shiftable<T>) {
        os << x + off;
    }
    else if constexpr (Range<T>) {
        const auto sep = NestedRange<T> ? delims.nested : delims.element;
        bool first = true;
        for (const auto& e : x) {
            if (not first) {
                os << sep;
            }
            first = false;
            write_shifted<off>(os, e, delims);
        }
    }
    else if constexpr (TupleLike<T>) {
        bool first = true;
        std::apply([&](const auto&... xs){
            auto out = [&](const auto& y) {
                if (not first) os << ' ';
                first = false;
                write_shifted<off>(os, y, delims);
            };
            (out(xs), ...);
        }, x);
    }
    else {
        os << x;
    }
}

template<int off, class Out, class T>
Out format_shifted(Out out, const T& x, delimiter_t delims = {}) {
    if constexpr (Shiftable<T>) {
        return std::format_to(out, "{}", x + off);
    }
    else if constexpr (Range<T>) {
        const auto sep = NestedRange<T> ? delims.nested : delims.element;
        bool first = true;
        for (const auto& e : x) {
            if (not first) {
                out = std::format_to(out, "{}", sep);
            }
            first = false;
            out = format_shifted<off>(out, e, delims);
        }
        return out;
    }
    else if constexpr (TupleLike<T>) {
        bool first = true;
        std::apply([&](const auto&... xs) {
            auto f = [&](const auto& y) {
                if (!first) *out++ = ' ';
                first = false;
                out = format_shifted<off>(out, y, delims);
            };
            (f(xs), ...);
        }, x);
        return out;
    }
    else {
        return std::format_to(out, "{}", x);
    }
}

}

template<int off>
struct shifted_istream {
    std::istream& is;

    template<int new_off>
    shifted_istream<new_off> operator>>(shifted_t<new_off>) {
        return {is};
    }

    template<class T>
    shifted_istream& operator>>(T& x) {
        detail::read_shifted<off>(is, x);
        return *this;
    }

    shifted_istream& operator>>(std::istream& (*manip)(std::istream&)) {
        manip(is);
        return *this;
    }

    shifted_istream& operator>>(std::ios_base& (*manip)(std::ios_base&)) {
        manip(is);
        return *this;
    }
};

template<int off>
struct shifted_ostream {
    std::ostream& os;
    delimiter_t delims{};

    template<int new_off>
    shifted_ostream<new_off> operator<<(shifted_t<new_off>) {
        return {os, delims};
    }

    shifted_ostream& operator<<(delimiter_t d) {
        delims = d;
        return *this;
    }

    template<class T>
    shifted_ostream& operator<<(const T& x) {
        detail::write_shifted<off>(os, x, delims);
        return *this;
    }

    shifted_ostream& operator<<(std::ostream& (*manip)(std::ostream&)) {
        manip(os);
        return *this;
    }

    shifted_ostream& operator<<(std::ios_base& (*manip)(std::ios_base&)) {
        manip(os);
        return *this;
    }
};

template<int off>
shifted_istream<off> operator>>(std::istream& is, shifted_t<off>) {
    return {is};
}

template<int off>
shifted_ostream<off> operator<<(std::ostream& os, shifted_t<off>) {
    return {os};
}

inline shifted_ostream<0> operator<<(std::ostream& os, delimiter_t d) {
    return {os, d};
}

}
}

namespace std {

template<int off, class T>
struct formatter<ad9::io::shifted_ref<off, T>, char> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const ad9::io::shifted_ref<off, T>& w, std::format_context& ctx) const {
        return ad9::io::detail::format_shifted<off>(ctx.out(), w.x, w.delims);
    }
};

}

namespace io = ad9::io;

#if defined(LOCAL)

#include "template/debug.hpp"

#else

#define Show(...)
#define Showln(...)
#define Dump(...)
#define Dumpln(...)
#define Debug(...)

#endif

#endif
