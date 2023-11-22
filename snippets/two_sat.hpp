#include <bits/stdc++.h>
#include <ranges>
#include <unistd.h>

using namespace std;
using Graph = vector<vector<uint>>;
using WeightedGraph = vector<vector<pair<int, int>>>;
using uint = unsigned;
using ll = long long;
using ull = unsigned long long;

#define rep(a, b) for (int a = 0; a < (b); ++a)
#define urep(a, b) for (uint a = 0; a < (b); ++a)
#define all(a) (a).begin(), (a).end()
#define endl '\n'

// nactl: start

// The node <i, b> means that variable i should have value b
using Node = pair<uint, bool>;

enum Decision {
    Yes,
    No,
    Maybe,
};

struct TwoSat {
    vector<vector<uint>> inner;
    vector<Decision> decision;

    static uint index_of(Node v) {
        return 2 * v.first + v.second;
    }
    static uint var_of(uint index) { return index / 2; }
    static bool is_true_node(uint v) { return v % 2 == 1; }
    static Node neg(Node v) { return {v.first, !v.second}; }
    static uint neg(uint v) {
        return index_of(neg({var_of(v), is_true_node(v)}));
    }

    bool is_node_satisfied(uint v) {
        return (decision[var_of(v)] == Yes) ==
               is_true_node(v);
    }

    TwoSat(uint size)
        : inner(2 * size)
        , decision(size, Maybe) {}

    uint new_variable() {
        inner.emplace_back();
        inner.emplace_back();
        decision.push_back(Maybe);
        return decision.size() - 1;
    }

    void insert_imp(Node a, Node b) {
        insert_imp(index_of(a), index_of(b));
    }
    void insert_imp(uint a, uint b) {
        inner[a].push_back(b);
        inner[neg(b)].push_back(neg(a));
    }

    void insert_all_but_one_request(span<Node> nodes) {
        // For each node, add a node that says that whether
        // any node up to it was not satisfied; then,
        // enforce that if node i is not satisfied, all
        // others have been so far
        vector<Node> cum_nodes(nodes.size());
        for (uint i = 0; i < cum_nodes.size(); ++i) {
            cum_nodes[i] = {new_variable(), true};
            insert_imp(neg(nodes[i]), cum_nodes[i]);
            if (i > 0) {
                insert_imp(cum_nodes[i - 1], cum_nodes[i]);
                insert_imp(cum_nodes[i - 1], nodes[i]);
            }
        }
    }

    bool solve() {
        // find SCCs, if a node is in same SCC as its
        // negated node, decision is not possible

        const auto inf = numeric_limits<uint>::max();

        uint scc_count = 0;
        vector<uint> scc_of(inner.size(), inf);
        vector<uint> disc(inner.size(), inf);
        vector<uint> some_node_in_scc;

        auto dfs = [&,
                    time = 0u,
                    st = vector<uint>(),
                    in_stack = vector(inner.size(), false)](
                       uint v, auto &rec) mutable -> uint {
            uint old_size = st.size();
            st.push_back(v);
            in_stack[v] = true;
            uint low = disc[v] = time++;
            for (auto v2 : inner[v]) {
                if (disc[v2] == inf)
                    low = min(low, rec(v2, rec));
                else if (in_stack[v2])
                    low = min(low, disc[v2]);
            }
            if (low == disc[v]) {
                for (uint i = old_size; i < st.size();
                     ++i) {
                    scc_of[st[i]] = scc_count;
                    in_stack[st[i]] = false;
                }
                some_node_in_scc.push_back(v);
                scc_count++;

                st.resize(old_size);
            }
            return low;
        };
        urep(v, inner.size()) if (disc[v] == inf)
            dfs(v, dfs);

        urep(var, inner.size() / 2) {
            if (scc_of[index_of({var, false})] ==
                scc_of[index_of({var, true})]) {
                return false;
            }
        }

        // otherwise, mark SCCs in any order
        for (uint start : some_node_in_scc) {
            if (decision[var_of(start)] != Maybe) continue;

            // mark all reachable as true
            auto mark_dfs = [&](uint v, auto &rec) -> void {
                assert(decision[var_of(v)] == Maybe);
                decision[var_of(v)] =
                    is_true_node(v) ? Yes : No;
                for (auto neigh : inner[v]) {
                    if (decision[var_of(neigh)] == Maybe)
                        rec(neigh, rec);
                    else
                        assert(is_node_satisfied(v) ==
                               is_node_satisfied(neigh));
                }
            };
            mark_dfs(start, mark_dfs);
        }

        assert(none_of(all(decision),
                       [](auto d) { return d == Maybe; }));
        return true;
    }
};
