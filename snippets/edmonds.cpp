#include <bits/stdc++.h>
#include <unistd.h>

#ifdef COMPPROG_LOCAL
#include <fmt/format.h>
#define dbg(...) fmt::print(stderr, __VA_ARGS__)
#define debug_assert assert
#else
#define dbg(...)
#define debug_assert(...)
#endif

using namespace std;
using uint = unsigned;
using ll = long long;
using ull = unsigned long long;

template <typename E = uint>
using Graph = vector<vector<E>>;
template <typename T>
using min_priority_queue =
    priority_queue<T, vector<T>, std::greater<T>>;

#define rep(a, b) for (int a = 0; a < (b); ++a)
#define urep(a, b) for (uint a = 0; a < (b); ++a)
#define all(a) (a).begin(), (a).end()
#define endl '\n'

// https://en.wikipedia.org/wiki/Edmonds%27_algorithm
// nactl: start
// Runtime: O(n * m), worst case O(n) recursive calls
template <typename C>
vector<uint> edmonds(Graph<pair<uint, C>> g, uint root) {
    if (g.size() == 1) return {root};

    const auto find_cycle =
        [](const auto &parent) -> optional<vector<uint>> {
        vector<uint> seen_from_start(parent.size(),
                                     parent.size());
        auto succ = [&](auto x) { return parent[x].first; };
        vector<uint> order;
        urep(start, parent.size()) {
            order.clear();
            auto cur = start;
            while (true) {
                if (seen_from_start[cur] < start) {
                    // seen in previous iteration
                    break;
                }

                if (seen_from_start[cur] == start) {
                    order.erase(order.begin(),
                                find(order.begin(),
                                     order.end(),
                                     cur));
                    return order;
                }

                order.push_back(cur);
                seen_from_start[cur] = start;
                if (succ(cur) == cur) break;
                cur = succ(cur);
            }
        }

        return {};
    };

    urep(v, g.size()) erase_if(g[v], [&](const auto &t) {
        return t.first == root;
    });

    const auto inf = numeric_limits<C>::max();
    vector<pair<uint, C>> parent(g.size(),
                                 make_pair(g.size(), inf));
    urep(u, g.size()) for (auto [v, w] : g[u]) {
        if (w < parent[v].second) parent[v] = {u, w};
    }
    urep(v, g.size()) assert(
        (parent[v].first == g.size()) == (v == root));
    parent[root].first = root;

    if (const auto cycle = find_cycle(parent)) {
        vector<char> is_in_cycle(g.size(), false);
        for (auto v : *cycle) is_in_cycle[v] = true;

        auto h = Graph<pair<uint, C>>(g.size() -
                                      cycle->size() + 1);
        const auto cycle_node = h.size() - 1;

        vector<uint> node_mapping(
            g.size(), numeric_limits<uint>::max()),
            reverse_mapping(g.size(),
                            numeric_limits<uint>::max());
        {
            uint next = 0;
            urep(v, g.size()) if (!is_in_cycle[v]) {
                node_mapping[v] = next++;
                reverse_mapping[node_mapping[v]] = v;
            }
        }

        vector<pair<uint, C>> incoming(h.size() - 1,
                                       {h.size(), inf}),
            outgoing(h.size() - 1, {h.size(), inf});

        urep(u, g.size()) for (auto [v, w] : g[u]) {
            if (!is_in_cycle[u] && is_in_cycle[v]) {
                if (w - parent[v].second <
                    incoming[node_mapping[u]].second)
                    incoming[node_mapping[u]] = {
                        v, w - parent[v].second};
            } else if (is_in_cycle[u] && !is_in_cycle[v]) {
                if (w < outgoing[node_mapping[v]].second)
                    outgoing[node_mapping[v]] = {u, w};
            } else if (!is_in_cycle[u] && !is_in_cycle[v]) {
                h[node_mapping[u]].push_back(
                    {node_mapping[v], w});
            }
        }

        urep(u, incoming.size()) {
            if (incoming[u].second < inf)
                h[u].push_back(
                    {cycle_node, incoming[u].second});
        }
        urep(v, outgoing.size()) {
            if (outgoing[v].second < inf)
                h[cycle_node].push_back(
                    {v, outgoing[v].second});
        }

        const auto h_parent =
            edmonds(move(h), node_mapping[root]);
        auto g_parent = vector<uint>(g.size(), g.size());
        urep(v, h_parent.size()) {
            const auto u = h_parent[v];
            if (u == cycle_node) {
                g_parent[reverse_mapping[v]] =
                    outgoing[v].first;
            } else if (v == cycle_node) {
                g_parent[incoming[u].first] =
                    reverse_mapping[u];
            } else {
                g_parent[reverse_mapping[v]] =
                    reverse_mapping[u];
            }
        }

        uint debug_count = 0;
        for (auto v : *cycle) {
            if (g_parent[v] != g.size()) {
                ++debug_count;
                continue;
            }

            g_parent[v] = parent[v].first;
        }
        assert(debug_count == 1);

        urep(v, g.size()) assert(g_parent[v] < g.size());
        assert(g_parent[root] == root);
        return g_parent;
    } else {
        auto parent_without_weight = vector<uint>(g.size());
        urep(v, g.size()) parent_without_weight[v] =
            parent[v].first;
        return parent_without_weight;
    }
}
// nactl: end

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);

    // https://judge.yosupo.jp/problem/directedmst
    uint n, m, root;
    cin >> n >> m >> root;
    Graph<pair<uint, ull>> g(n);
    urep(_, m) {
        uint u, v;
        ull w;
        cin >> u >> v >> w;
        g[u].push_back({v, w});
    }

    const auto parent = edmonds(g, root);
    ull total_weight = 0;
    urep(u, n) for (auto [v, w] : g[u]) if (u == parent[v])
        total_weight += w;
    cout << total_weight << endl;
    for (auto p : parent) cout << p << " ";
    cout << endl;
}
