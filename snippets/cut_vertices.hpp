// returns earliest discover time
// reachable via 1 edge from my subtree
int dfs(int v) {
    int low = disc[v] = time++;
    int kids = 0;
    for (int v2 : adj[v]) {
        int st_pos = size(st);
        if (disc[v2] == -1) { // tree edge
            ++kids;
            int low2 = dfs(v2, v);
            low = min(low, low2);
            if (low2 >= disc[v]) { // exiting comp
                comps.emplace_back(begin(st) + st_pos, end(st));
                st.resize(st_pos);

                if (p[v] != -1 && kids > 1) {
                    // v is cut-vertex
                }
            }
        }
        if (disc[v2] < disc[v]) // from below
            st.push_back({v, v2});
        low = min(low, disc[v2]);
    }
    return low;
}
