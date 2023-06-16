vector<ull> dist(n, inf);
min_priority_queue<pair<ull, uint>> pq;

dist[start] = 0;
pq.push({dist[start], start});

while (!pq.empty()) {
    auto [weight_in, u] = pq.top();
    pq.pop();
    // u might be added multiple times for longer path, but we only care about the first time it
    // is removed from the queue
    if (weight_in > dist[u]) continue;

    for (auto [v, w] : g[u]) {
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            pq.push({dist[v], v});
        }
    }
}
