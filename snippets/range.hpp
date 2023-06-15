struct Range {
    ull start, end;

    // static Range from_ends(ull start, ull end) { return {start, end}; }

    bool operator==(const Range &rhs) const {
        return (is_empty() && rhs.is_empty()) || (start == rhs.start && end == rhs.end);
    }

    bool is_subset_of(Range other) const { return other.start <= start && end <= other.end; }
    Range intersect_with(Range other) const {
        return Range{
            .start = max(start, other.start),
            .end = min(end, other.end),
        };
    }
    bool is_empty() const { return start >= end; }
    ull size() const {
        if (is_empty()) return 0;
        else return end - start;
    }
    bool contains(ull point) const { return start <= point && point < end; }
    pair<Range, Range> split_at_mid() const {
        assert(!is_empty());
        auto m = start + (end - start) / 2;
        return {{start, m}, {m, end}};
    }
};
