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

template <typename E = uint> using Graph = vector<vector<E>>;
template <typename T>
using min_priority_queue = priority_queue<T, vector<T>, std::greater<T>>;

#define rep(a, b) for (int a = 0; a < (b); ++a)
#define urep(a, b) for (uint a = 0; a < (b); ++a)
#define all(a) (a).begin(), (a).end()
#define endl '\n'

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(10);
}
