auto lift_by = vector(bit_width(g.size()), vector(g.size(), root));
urep(v, n) lift_by[0][v] = parent[v];
lift_by[0][root] = root;
urep(prev_height, lift_by.size() - 1)
    urep(v, n) lift_by[prev_height + 1][v] = lift_by[prev_height][lift_by[prev_height][v]];

auto lca = [&](uint u, uint v) {
    if (depth[u] > depth[v]) swap(u, v);
    const auto depth_diff = depth[v] - depth[u];
    urep(i, lift_by.size()) if (depth_diff & (1 << i)) u = lift_by[i][u];
    debug_assert(depth[u] == depth[v]);
    if (u == v) return u;
    for (int i = lift_by.size() - 1; i >= 0; --i)
        if (lift_by[i][u] != lift_by[i][v]) u = lift_by[i][u], v = lift_by[i][v];
    return parent[u];
};
