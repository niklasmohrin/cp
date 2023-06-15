struct Edge {
    uint from, to;
    ll cap;
    ll flow = 0;
    Edge *rev = nullptr;
};

Graph<Edge *> g(n);

auto add_edge = [&](uint u, uint v, ll cap) {
    if (cap == 0) return;
    auto uv = new Edge{u, v, cap};
    auto vu = new Edge{v, u, 0};
    uv->rev = vu;
    vu->rev = uv;
    g[u].push_back(uv);
    g[v].push_back(vu);
};

ll total_flow = 0;
while (true) {
    // build L
    const auto inf = numeric_limits<uint>::max();
    vector<uint> dist(g.size(), inf);
    dist[source] = 0;
    queue<uint> q;
    q.push(source);
    while (!q.empty()) {
        auto u = q.front();
        q.pop();
        for (auto e : g[u]) {
            if (dist[e->to] == inf && e->flow < e->cap) {
                q.push(e->to);
                dist[e->to] = dist[u] + 1;
            }
        }
    }
    if (dist[target] == inf) break;

    // augment
    vector<uint> current_edge_index(g.size(), 0);

    auto dfs = [&](uint u, ll bottleneck_so_far, auto &rec) -> ll {
        if (u == target) return bottleneck_so_far;
        for (auto &i = current_edge_index[u]; i < g[u].size(); ++i) {
            auto e = g[u][i];
            if (e->flow == e->cap) continue;
            if (dist[e->to] != dist[u] + 1) continue;
            auto pushed = rec(e->to, min(bottleneck_so_far, e->cap - e->flow), rec);
            if (pushed == 0) continue;
            e->flow += pushed;
            e->rev->flow -= pushed;
            return pushed;
        }
        return 0;
    };

    while (true) {
        auto bottleneck = dfs(source, numeric_limits<ll>::max(), dfs);
        if (bottleneck == 0) break;
        total_flow += bottleneck;
    }
}
