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

// nactl: start

using Matching = vector<pair<uint, uint>>;

Matching gale_shapley(
    const vector<vector<uint>> &preference_lists_left,
    const vector<vector<uint>> &preference_lists_right) {
    auto right_rank =
        vector(preference_lists_right.size(),
               vector(preference_lists_left.size(), 0u));
    urep(w, preference_lists_right.size()) {
        right_rank[w].assign(
            preference_lists_left.size(),
            preference_lists_right[w].size());
        urep(i, preference_lists_right[w].size())
            right_rank[w][preference_lists_right[w][i]] = i;
    }

    queue<uint> free_left;
    urep(u, preference_lists_left.size()) free_left.push(u);

    vector<uint> next_index(preference_lists_left.size(),
                            0);
    vector<uint> current_partner_index(
        preference_lists_right.size());
    urep(w, preference_lists_right.size())
        current_partner_index[w] =
            preference_lists_right[w].size();

    while (!free_left.empty()) {
        auto u = free_left.front();
        free_left.pop();

        auto asking_index = next_index[u]++;
        if (asking_index == preference_lists_left[u].size())
            continue;

        auto w = preference_lists_left[u][asking_index];
        if (current_partner_index[w] ==
                preference_lists_right[w].size() ||
            right_rank[w][u] <
                right_rank[w][current_partner_index[w]]) {
            if (current_partner_index[w] <
                preference_lists_right[w].size()) {
                auto cur_partner = preference_lists_right
                    [w][current_partner_index[w]];
                free_left.push(cur_partner);
            }
            current_partner_index[w] = right_rank[w][u];
        }
    }

    Matching m;
    urep(w, preference_lists_right.size()) {
        if (current_partner_index[w] !=
            preference_lists_right[w].size()) {
            const auto u = preference_lists_right
                [w][current_partner_index[w]];
            m.emplace_back(u, w);
        }
    }
    return m;
}

// nactl: end

int main() {
    uint l, r;
    cin >> l >> r;
    vector<vector<uint>> preference_lists_left(l),
        preference_lists_right(r);
    urep(u, l) {
        uint k;
        cin >> k;
        preference_lists_left[u].resize(k);
        urep(i, k) cin >> preference_lists_left[u][i];
    }

    urep(w, r) {
        uint k;
        cin >> k;
        preference_lists_right[w].resize(k);
        urep(i, k) cin >> preference_lists_right[w][i];
    }

    auto matching = gale_shapley(preference_lists_left,
                                 preference_lists_right);

    for (auto [u, w] : matching)
        cout << u << " " << w << endl;
}
