
#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <limits>
#include <map>
#include <type_traits>
#include <utility>

template <std::integral T>
    requires(not std::same_as<T, bool>)
class IntervalSet {
  public:
    using U = std::make_unsigned_t<T>;
    using const_iterator = typename std::map<T, T>::const_iterator;

  private:
    std::map<T, T> intervals;
    U sum = 0;

    static constexpr U length(T l, T r) {
        return static_cast<U>(static_cast<U>(r) - static_cast<U>(l));
    }

  public:
    const_iterator begin() const noexcept {
        return intervals.cbegin();
    }

    const_iterator end() const noexcept {
        return intervals.cend();
    }

    const_iterator cover(T l, T r) const {
        assert(l <= r);
        if (l == r) {
            return end();
        }
        auto it = intervals.upper_bound(l);
        if (it == begin()) return end();
        it = std::prev(it);
        return r <= it->second ? it : end();
    }

    const_iterator cover(T x) const {
        auto it = intervals.upper_bound(x);
        if (it == begin()) return end();
        it = std::prev(it);
        return x < it->second ? it : end();
    }

    bool contains(T l, T r) const {
        assert(l <= r);
        return l == r || cover(l, r) != end();
    }

    bool contains(T x) const {
        return cover(x) != end();
    }

    const_iterator intersect(T l, T r) const {
        assert(l <= r);
        if (l == r) {
            return end();
        }
        auto it = intervals.lower_bound(l);
        if (it != begin() && std::prev(it)->second > l) {
            return std::prev(it);
        }
        return it != end() && it->first < r ? it : end();
    }

    U insert(T l, T r) {
        assert(l <= r);
        if (l == r) return 0;
        auto it = intervals.lower_bound(l);
        if (it != intervals.begin() && std::prev(it)->second >= l) {
            it = std::prev(it);
        }
        U covered = 0;
        while (it != intervals.end() && it->first <= r) {
            l = std::min(l, it->first);
            r = std::max(r, it->second);
            covered += length(it->first, it->second);
            it = intervals.erase(it);
        }
        intervals.emplace_hint(it, l, r);
        U added = length(l, r) - covered;
        sum += added;
        return added;
    }

    U insert(T x) {
        assert(x < std::numeric_limits<T>::max());
        return insert(x, x + 1);
    }

    U erase(T l, T r) {
        assert(l <= r);
        if (l == r) return 0;
        auto it = intervals.lower_bound(l);
        if (it != intervals.begin() && std::prev(it)->second > l) {
            it = std::prev(it);
        }
        U removed = 0;
        while (it != intervals.end() && it->first < r) {
            const auto [a, b] = *it;
            removed += length(std::max(a, l), std::min(b, r));
            it = intervals.erase(it);
            if (a < l) {
                intervals.emplace_hint(it, a, l);
            }
            if (r < b) {
                intervals.emplace_hint(it, r, b);
                break;
            }
        }
        sum -= removed;
        return removed;
    }

    U erase(T x) {
        assert(x < std::numeric_limits<T>::max());
        return erase(x, x + 1);
    }

    T mex(T x = 0) const {
        auto it = intervals.upper_bound(x);
        if (it != intervals.begin() && x < std::prev(it)->second) {
            return std::prev(it)->second;
        }
        return x;
    }

    std::size_t size() const noexcept {
        return intervals.size();
    }

    U length() const noexcept {
        return sum;
    }
};
