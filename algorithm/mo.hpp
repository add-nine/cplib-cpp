#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <ranges>
#include <vector>

class Mo {
    struct Query {
        std::uint64_t key;
        int l, r, id;
    };

    int B;
    std::vector<Query> queries;

  public:
    explicit Mo(int N, int Q) {
        assert(N > 0 && Q > 0);
        B = std::max<int>(1, N / std::sqrt((2.0 / 3.0) * Q));
        queries.reserve(Q);
    }

    int push(int l, int r) {
        assert(0 <= l && l <= r);
        int id = static_cast<int>(queries.size());
        std::uint64_t block = l / B;
        std::uint32_t order = r;
        std::uint64_t key = (block << 32) | (block & 1 ? ~order : order);
        queries.emplace_back(key, l, r, id);
        return id;
    }

    template <class AddFL, class AddFR, class DelFL, class DelFR, class F>
    void run(AddFL&& add_l, AddFR&& add_r, DelFL&& del_l, DelFR&& del_r, F&& record) {
        if (queries.empty()) {
            return;
        }
        std::ranges::sort(queries, [](const Query& a, const Query& b) {
            return a.key != b.key ? a.key < b.key : a.l < b.l;
        });
        int l = 0, r = 0;
        for (const Query& q : queries) {
            while (l > q.l) add_l(--l);
            while (r < q.r) add_r(r++);
            while (l < q.l) del_l(l++);
            while (r > q.r) del_r(--r);
            record(q.id);
        }
    }

    template <class AddF, class DelF, class F>
    void run(AddF&& add, DelF&& del, F&& record) {
        run(add, add, del, del, record);
    }
};
