#pragma once

/*
requires the I/O utilities defined in template.cpp.
*/

#include <iostream>
#include <print>

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