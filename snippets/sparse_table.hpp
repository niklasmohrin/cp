template <typename T, auto op, T neutral_element> struct SparseTable {
    // op needs to
    // - be associative
    // - be idempotent
    // - have neutral element neutral_element

    uint n;
    vector<T> aggs;

    uint idx(uint i, uint row) const { return n * row + i; }

    SparseTable(span<T const> vals) : n(vals.size()), aggs(n * (bit_width(n) + 1)) {

        ranges::copy(vals, aggs.begin());

        for (uint row = 1, agg_size = 2; agg_size < n; ++row, agg_size <<= 1) {
            for (uint i = 0; i + (agg_size / 2) < n; ++i) {
                aggs[idx(i, row)] =
                    op(aggs[idx(i, row - 1)], aggs[idx(i + (agg_size / 2), row - 1)]);
            }
        }
    }

    // zero-based
    T range_query(Range range) const {
        if (range.is_empty()) return neutral_element;
        auto agg_index = bit_width(range.size()) - 1;
        auto agg_size = 1u << agg_index;
        return op(aggs[idx(range.start, agg_index)], aggs[idx(range.end - agg_size, agg_index)]);
    }
};
