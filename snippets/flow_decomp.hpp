vector<uint> current_edge_index(n, 0);
vector<pair<ll, vector<uint>>> paths;

while (true) {
    vector<Edge *> path_edges;
    vector<uint> path;
    auto dfs = [&](uint u, ll bottleneck_so_far, auto &rec) -> ll {
        if (u == target) return bottleneck_so_far;
        for (auto &i = current_edge_index[u]; i < g[u].size(); ++i) {
            auto e = g[u][i];
            if (e->cap != 0 && e->flow > 0) {
                path_edges.push_back(e);
                path.push_back(u);
                return rec(e->to, min(bottleneck_so_far, e->flow), rec);
            }
        }
        return 0;
    };
    auto bn = dfs(source, numeric_limits<ll>::max(), dfs);
    if (bn == 0) break;
    for (auto e : path_edges) e->flow -= bn, e->rev->flow += bn;

    path.push_back(target);
    paths.push_back(make_pair(bn, move(path)));
}
