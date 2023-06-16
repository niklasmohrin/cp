auto dist = vector(g.size(), vector(g.size(), inf));
for (auto [u, v, w] : edges) dist[u][v] = dist[v][u] = w;
urep(k, n) {
    urep(u, n) {
        urep(v, n) {
            if (dist[u][k] != inf && dist[k][v] != inf) {
                dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
            }
        }
    }
}
