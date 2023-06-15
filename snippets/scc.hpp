uint scc_count = 0;
vector<vector<uint>> scss;
vector<uint> scc_of(g.size(), inf);
vector<uint> disc(g.size(), inf);
vector<uint> some_node_in_scc;

auto dfs = [&, time = 0u, st = vector<uint>(), in_stack = vector(g.size(), false)](
               uint v, auto &rec) mutable -> uint {
    uint old_size = st.size();
    st.push_back(v);
    in_stack[v] = true;
    uint low = disc[v] = time++;
    for (auto v2 : g[v]) {
        if (disc[v2] == inf) low = min(low, rec(v2, rec));
        else if (in_stack[v2]) low = min(low, disc[v2]);
    }
    if (low == disc[v]) {
        for (uint i = old_size; i < st.size(); ++i) {
            scc_of[st[i]] = scc_count;
            in_stack[st[i]] = false;
        }
        some_node_in_scc.push_back(v);
        scc_count++;

        sccs.emplace_back(st.begin() + old_size, st.end());
        st.resize(old_size);
    }
    return low;
};
urep(v, g.size()) if (disc[v] == inf) dfs(v, dfs);
