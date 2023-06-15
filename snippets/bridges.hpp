vector<uint> st;
vector<vector<uint>> components;
vector<int> depth_of(n, -1);
vector<pair<uint, uint>> bridges;

auto dfs = [&](uint v, uint parent, uint depth, auto &rec) -> uint {
    auto old_size = size(st);
    st.push_back(v);

    uint up = 0;
    depth_of[v] = depth;
    for (auto neigh : g[v]) {
        if (depth_of[neigh] == -1) up += rec(neigh, v, depth + 1, rec);
        up += (depth_of[neigh] < depth);
        up -= (depth_of[neigh] > depth);
    }

    if (up == 1) {
        bridges.emplace_back(parent, v);
    }

    if (depth == 0 || up == 1) {
        components.emplace_back(st.begin() + old_size, st.end());
        st.resize(old_size);
    }

    return up;
};
dfs(0, 0, 0, dfs);
