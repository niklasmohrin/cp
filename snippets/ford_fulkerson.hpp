template <typename T> using Mat = vector<vector<T>>;
using Cap = ull;

auto bfs(const Mat<Cap> &g, vector<int> &parent, const uint s, const uint t, const Cap min_cap) {
    vector<bool> seen(g.size(), false);

    queue<uint> q;
    q.push(s);
    seen[s] = true;
    parent[s] = -1;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop();

        urep(neigh, g.size()) if (!seen[neigh] && g[cur][neigh] >= min_cap) {
            parent[neigh] = cur;
            if (neigh == t) {
                return true;
            }

            seen[neigh] = true;
            q.push(neigh);
        }
    }

    return false;
}

auto max_flow(Mat<Cap> &g, const uint s, const uint t) {
    Cap flow = 0;
    vector<int> parent(g.size());

    Cap min_cap = *ranges::max_element(g[s]);

    while (min_cap) {
        while (bfs(g, parent, s, t, min_cap)) {
            uint prev = t, cur;
            Cap bottleneck = numeric_limits<Cap>::max();

            while (prev != s) {
                cur = parent[prev];
                bottleneck = min(bottleneck, g[cur][prev]);
                prev = cur;
            }

            prev = t;
            while (prev != s) {
                cur = parent[prev];
                g[cur][prev] -= bottleneck;
                g[prev][cur] += bottleneck;
                prev = cur;
            }
            flow += bottleneck;
        }
        min_cap /= 2;
    }

    return flow;
}
