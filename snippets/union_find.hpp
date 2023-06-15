struct UnionFind {
    vector<uint> p, size;

    UnionFind(uint n) : p(n), size(n, 1) { iota(p.begin(), p.end(), 0); }

    auto raw_key(uint i) const {
        while (p[i] != i) i = p[i];
        return i;
    }
    void update_key(uint i, const uint k) {
        while (p[i] != k) i = exchange(p[i], k);
    }
    auto key(uint i) {
        const auto k = raw_key(i);
        update_key(i, k);
        return k;
    }
    void unite(uint i, uint j) {
        const auto ki = raw_key(i), kj = raw_key(j);
        if (ki == kj) return;
        const auto k = size[ki] >= size[kj] ? ki : kj;
        size[k] = size[ki] + size[kj];
        update_key(i, k);
        update_key(j, k);
    }
};
