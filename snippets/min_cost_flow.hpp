struct Edge {
    const uint from;
    const uint to;
    ll flow;
    const ll cap;
    const ll cost;
    Edge *rev = nullptr;
};

auto add_edge = [&g, &all_edges](uint u, uint v, ll cap, ll cost) mutable {
    auto *uv = new Edge(u, v, 0, cap, cost);
    auto *vu = new Edge(v, u, 0, 0, -cost);
    uv->rev = vu;
    vu->rev = uv;
    g[u].push_back(uv);
    g[v].push_back(vu);
};

ll total_flow = 0;

vector<ll> johnson_offset(g.size(), inf);
{
    // Bellman Ford to get johnson offsets
    auto &dist = johnson_offset;
    dist[source] = 0;
    queue<uint> q;
    q.push(source);
    vector<bool> in_q(g.size());
    in_q[source] = true;

    while (!q.empty()) {
        auto u = q.front();
        q.pop();
        in_q[u] = false;
        for (auto e : g[u]) {
            if (e->flow == e->cap) continue;
            if (dist[e->from] == inf) continue;
            if (dist[e->from] + e->cost >= dist[e->to]) continue;
            dist[e->to] = dist[e->from] + e->cost;
            if (in_q[e->to]) continue;
            q.push(e->to);
            in_q[e->to] = true;
        }
    }
}

while (true) {
    // try to find shortest path according to cost
    vector<ll> dist(g.size(), inf);
    dist[source] = 0;
    vector<Edge *> incoming(g.size());
    priority_queue<pair<ll, uint>, vector<pair<ll, uint>>, greater<>> q;
    q.push({0, source});
    vector<bool> done(g.size());
    while (!q.empty()) {
        auto [_, u] = q.top();
        q.pop();
        done[u] = true;
        for (auto e : g[u]) {
            if (done[e->to]) continue;
            if (e->flow == e->cap) continue;
            if (dist[e->from] == inf) continue;
            const auto john_cost = e->cost + johnson_offset[e->from] - johnson_offset[e->to];
            if (dist[e->from] + john_cost >= dist[e->to]) continue;
            dist[e->to] = dist[e->from] + john_cost;
            incoming[e->to] = e;
            q.push({dist[e->to], e->to});
        }
    }
    if (dist[target] == inf) break;

    urep(v, g.size()) johnson_offset[v] += dist[v] - dist[target];

    // find bottleneck
    ll bottleneck = inf;
    for (auto v = target; v != source; v = incoming[v]->from)
        bottleneck = min(bottleneck, incoming[v]->cap - incoming[v]->flow);
    total_flow += bottleneck;
    // augment
    for (auto v = target; v != source; v = incoming[v]->from) {
        incoming[v]->flow += bottleneck;
        incoming[v]->rev->flow -= bottleneck;
    }
}

ll total_cost = 0;
for (auto &row : g)
    for (auto e : row) total_cost += e->flow * e->cost;
total_cost /= 2;
