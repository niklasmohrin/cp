vector<uint> topo;
urep(u, n) if (!in_deg[u]) topo.push_back(u);
urep(i, n) for (auto u : g[topo[i]]) if (!--in_deg[u]) topo.push_back(u);
