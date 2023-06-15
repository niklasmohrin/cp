#include <bits/stdc++.h>

struct Range {
    uint start, end;
};

using namespace std;
using uint = unsigned;

// nactl: start

template <typename T, typename Op, T neutral_element> struct SegmentTree {
    Op op;
    uint n;
    vector<T> tree;
    vector<T> lazy;

    SegmentTree(uint _n)
        : n(bit_ceil(_n))
        , tree(2 * n + 1, neutral_element)
        , lazy(2 * n + 1, neutral_element) {}

    bool is_leaf(uint v) const { return n <= v && v < 2 * n; }
    void recalc(uint v) { tree[v] = op(tree[left_child(v)], tree[right_child(v)]); }
    void recalc_all_parents(uint v) {
        v = parent(v);
        while (root <= v) recalc(v), v = parent(v);
    }
    static const uint root = 1;
    static uint parent(uint v) { return v / 2; }
    static uint left_child(uint v) { return 2 * v; }
    static uint right_child(uint v) { return 2 * v + 1; }

    void push_lazy(uint v) {
        if (lazy[v] == neutral_element) return;

        apply_lazy(left_child(v), lazy[v]);
        apply_lazy(right_child(v), lazy[v]);
        lazy[v] = neutral_element;
    }
    void apply_lazy(uint v, T value) {
        tree[v] = op(tree[v], value);
        if (!is_leaf(v)) lazy[v] = op(lazy[v], value);
    }

    void push_all_lazy(uint v) {
        for (uint bits_to_shift = bit_width(v) - 1; bits_to_shift > 0; --bits_to_shift)
            push_lazy(v >> bits_to_shift);
    }

    void range_update(Range range, T value) {
        push_all_lazy(n + range.start - 1);
        push_all_lazy(n + range.end - 2);
        for (auto l = n + range.start - 1, r = n + range.end - 1; l < r;
             l = parent(l), r = parent(r)) {
            if (l & 1) apply_lazy(l++, value);
            if (r & 1) apply_lazy(--r, value);
        }
        auto first_touched = [](auto x) { return x >> countr_zero(x); };
        recalc_all_parents(first_touched(n + range.start - 1));
        recalc_all_parents(first_touched(n + range.end - 1) - 1);
    }
    T range_query(Range range) {
        auto agg = neutral_element;
        push_all_lazy(n + range.start - 1);
        push_all_lazy(n + range.end - 2);
        for (auto l = n + range.start - 1, r = n + range.end - 1; l < r;
             l = parent(l), r = parent(r)) {
            if (l & 1) agg = op(tree[l++], agg);
            if (r & 1) agg = op(agg, tree[--r]);
        }
        return agg;
    }
};
