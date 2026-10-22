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

#define SHOW(x) std::print(std::cerr, "{}: {}, ", #x, io::raw(x))
#define SHOWLN(x) std::println(std::cerr, "{}: {}", #x, io::raw(x))
#define SHOW_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME

#define Show(...) SHOW_IMPL(__VA_ARGS__, SHOW8, SHOW7, SHOW6, SHOW5, SHOW4, SHOW3, SHOW2, SHOW1)(__VA_ARGS__)

#define SHOW1(x1) do { SHOW(x1); } while (false)
#define SHOW2(x1, x2) do { SHOW(x1); SHOW(x2); } while (false)
#define SHOW3(x1, x2, x3) do { SHOW(x1); SHOW(x2); SHOW(x3); } while (false)
#define SHOW4(x1, x2, x3, x4) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); } while (false)
#define SHOW5(x1, x2, x3, x4, x5) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); } while (false)
#define SHOW6(x1, x2, x3, x4, x5, x6) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOW(x6); } while (false)
#define SHOW7(x1, x2, x3, x4, x5, x6, x7) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOW(x6); SHOW(x7); } while (false)
#define SHOW8(x1, x2, x3, x4, x5, x6, x7, x8) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOW(x6); SHOW(x7); SHOW(x8); } while (false)

#define Showln(...) SHOW_IMPL(__VA_ARGS__, SHOWLN8, SHOWLN7, SHOWLN6, SHOWLN5, SHOWLN4, SHOWLN3, SHOWLN2, SHOWLN1)(__VA_ARGS__)

#define SHOWLN1(x1) do { SHOWLN(x1); } while (false)
#define SHOWLN2(x1, x2) do { SHOW(x1); SHOWLN(x2); } while (false)
#define SHOWLN3(x1, x2, x3) do { SHOW(x1); SHOW(x2); SHOWLN(x3); } while (false)
#define SHOWLN4(x1, x2, x3, x4) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOWLN(x4); } while (false)
#define SHOWLN5(x1, x2, x3, x4, x5) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOWLN(x5); } while (false)
#define SHOWLN6(x1, x2, x3, x4, x5, x6) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOWLN(x6); } while (false)
#define SHOWLN7(x1, x2, x3, x4, x5, x6, x7) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOW(x6); SHOWLN(x7); } while (false)
#define SHOWLN8(x1, x2, x3, x4, x5, x6, x7, x8) do { SHOW(x1); SHOW(x2); SHOW(x3); SHOW(x4); SHOW(x5); SHOW(x6); SHOW(x7); SHOWLN(x8); } while (false)

#define DUMP(x) std::print(std::cerr, "{} ", io::raw(x))
#define DUMPLN(x) std::println(std::cerr, "{}", io::raw(x))

#define Dump(...) SHOW_IMPL(__VA_ARGS__, DUMP8, DUMP7, DUMP6, DUMP5, DUMP4, DUMP3, DUMP2, DUMP1)(__VA_ARGS__)

#define DUMP1(x1) do { DUMP(x1); } while (false)
#define DUMP2(x1, x2) do { DUMP(x1); DUMP(x2); } while (false)
#define DUMP3(x1, x2, x3) do { DUMP(x1); DUMP(x2); DUMP(x3); } while (false)
#define DUMP4(x1, x2, x3, x4) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); } while (false)
#define DUMP5(x1, x2, x3, x4, x5) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); } while (false)
#define DUMP6(x1, x2, x3, x4, x5, x6) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMP(x6); } while (false)
#define DUMP7(x1, x2, x3, x4, x5, x6, x7) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMP(x6); DUMP(x7); } while (false)
#define DUMP8(x1, x2, x3, x4, x5, x6, x7, x8) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMP(x6); DUMP(x7); DUMP(x8); } while (false)

#define Dumpln(...) SHOW_IMPL(__VA_ARGS__, DUMPLN8, DUMPLN7, DUMPLN6, DUMPLN5, DUMPLN4, DUMPLN3, DUMPLN2, DUMPLN1)(__VA_ARGS__)

#define DUMPLN1(x1) do { DUMPLN(x1); } while (false)
#define DUMPLN2(x1, x2) do { DUMP(x1); DUMPLN(x2); } while (false)
#define DUMPLN3(x1, x2, x3) do { DUMP(x1); DUMP(x2); DUMPLN(x3); } while (false)
#define DUMPLN4(x1, x2, x3, x4) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMPLN(x4); } while (false)
#define DUMPLN5(x1, x2, x3, x4, x5) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMPLN(x5); } while (false)
#define DUMPLN6(x1, x2, x3, x4, x5, x6) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMPLN(x6); } while (false)
#define DUMPLN7(x1, x2, x3, x4, x5, x6, x7) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMP(x6); DUMPLN(x7); } while (false)
#define DUMPLN8(x1, x2, x3, x4, x5, x6, x7, x8) do { DUMP(x1); DUMP(x2); DUMP(x3); DUMP(x4); DUMP(x5); DUMP(x6); DUMP(x7); DUMPLN(x8); } while (false)

#define Debug(...) do { __VA_ARGS__; } while (false)

#else

#define Show(...)
#define Showln(...)
#define Dump(...)
#define Dumpln(...)
#define Debug(...)

#endif

#endif
