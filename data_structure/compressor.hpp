#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

template <std::totally_ordered T>
class Compressor {
  public:
    using const_iterator = typename std::vector<T>::const_iterator;

  private:
    std::vector<T> data;

  public:
    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_reference_t<R>, T>
    explicit Compressor(R&& vals) : data(std::from_range, std::forward<R>(vals)) {
        std::ranges::sort(data);
        data.erase(std::ranges::unique(data).begin(), data.end());
    }

    const_iterator begin() const noexcept {
        return data.cbegin();
    }

    const_iterator end() const noexcept {
        return data.cend();
    }

    std::size_t size() const noexcept {
        return data.size();
    }

    int operator()(const T& val) const {
        auto it = std::ranges::lower_bound(data, val);
        assert(it != data.end() && *it == val);
        return std::ranges::distance(data.begin(), it);
    }
};

template <std::ranges::input_range R>
Compressor(R&&) -> Compressor<std::ranges::range_value_t<R>>;