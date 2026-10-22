
#pragma once

#include <algorithm>
#include <cassert>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

class UnionFind {
    struct Node {
        int parent = -1;
        int to = -1;
    };
    std::vector<Node> tree;

  public:
    UnionFind() = default;

    explicit UnionFind(std::size_t N) : tree(N) {
        std::ranges::iota(tree | std::views::transform(&Node::to), 0);
    }

    int size(int u) {
        assert(0 <= u && u < std::ssize(tree));
        return -tree[find(u)].parent;
    }

    int find(int u) {
        assert(0 <= u && u < std::ssize(tree));
        return (tree[u].parent < 0 ? u : tree[u].parent = find(tree[u].parent));
    }

    bool unite(int u, int v) {
        assert(0 <= u && u < std::ssize(tree));
        assert(0 <= v && v < std::ssize(tree));
        if ((u = find(u)) == (v = find(v))) {
            return false;
        }
        if (size(u) < size(v)) std::swap(u, v);
        tree[u].parent += tree[v].parent;
        tree[v].parent = u;
        std::swap(tree[u].to, tree[v].to);
        return true;
    }

    bool same(int u, int v) {
        assert(0 <= u && u < std::ssize(tree));
        assert(0 <= v && v < std::ssize(tree));
        return find(u) == find(v);
    }

    std::vector<int> group(int u) const {
        assert(0 <= u && u < std::ssize(tree));
        std::vector<int> result{u};
        for (int v = u; tree[v].to != u; v = tree[v].to) {
            result.push_back(tree[v].to);
        }
        return result;
    }

    std::vector<std::vector<int>> groups() const {
        std::vector<std::vector<int>> result;
        for (int u : std::views::iota(0, std::ssize(tree))) {
            if (tree[u].parent < 0) result.push_back(group(u));
        }
        return result;
    }
};
