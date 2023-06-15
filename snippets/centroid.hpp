uint find_centroid(Graph<> &g, const uint n, const uint root, span<Data const> data) {
    uint centroid = root;

check:
    for (uint i = 0; i < g[centroid].size();) {
        const auto child = g[centroid][i];
        if (data[child].deleted) {
            swap_remove(g[centroid], i);
            continue;
        }
        ++i;

        if (child != data[centroid].parent && 2 * data[child].sz > n) {
            centroid = child;
            goto check;
        }
    }

    return centroid;
}
