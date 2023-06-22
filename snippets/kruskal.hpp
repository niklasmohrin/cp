struct Edge {
    ll w;
    uint u, v;

    auto operator<=>(const Edge &) const = default;
};

vector<Edge> kruskal(uint n, vector<Edge> edges) {
    ranges::sort(edges);

    vector<Edge> mst;
    UnionFind uf(n);
    for (auto edge : edges) {
        if (uf.key(edge.u) == uf.key(edge.v)) continue;
        uf.unite(edge.u, edge.v);
        mst.push_back(edge);
    }
    return mst;
}
