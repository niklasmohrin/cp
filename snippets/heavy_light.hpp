const auto inf = numeric_limits<uint>::max();
vector<uint> size(n, 0), parent(n, inf), depth(n, inf);
vector<uint> heavy_child(n, inf);
auto dfs1 = [&](auto v, auto &rec) -> void {
    size[v] = 1;
    for (auto child : g[v])
        if (child != parent[v]) {
            parent[child] = v;
            depth[child] = depth[v] + 1;
            rec(child, rec);
            size[v] += size[child];
            if (heavy_child[v] == inf || size[heavy_child[v]] < size[child]) heavy_child[v] = child;
        }
};
const uint root = 0;
depth[root] = 0;
parent[root] = root;
dfs1(root, dfs1);

// heavy light
vector<uint> head(n), pos(n);
vector<uint> initial_values(n);
auto dfs2 = [&, next_index = 1u](uint v, uint cur_head, auto &rec) mutable -> void {
    head[v] = cur_head;
    pos[v] = next_index++;
    initial_values[pos[v] - 1] = value_for_node[v];
    if (heavy_child[v] != inf) rec(heavy_child[v], cur_head, rec);
    for (auto child : g[v])
        if (child != parent[v] && child != heavy_child[v]) rec(child, child, rec);
};
dfs2(root, root, dfs2);

SegmentTree seg(initial_values);

uint u = a, v = b;
SegmentTree::Node ans = SegmentTree::neutral_element;
for (; head[u] != head[v]; v = parent[head[v]]) {
    if (depth[head[u]] > depth[head[v]]) swap(u, v);
    ans = SegmentTree::op(ans, seg.range_query({pos[head[v]], pos[v] + 1}));
}
if (depth[u] > depth[v]) swap(u, v);
ans = SegmentTree::op(ans, seg.range_query({pos[u], pos[v] + 1}));
