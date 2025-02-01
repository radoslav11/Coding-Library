#include <bits/stdc++.h>
#include <coding_library/graph/dsu.hpp>
using namespace std;

class UndirectedGraph {
  public:
    int n, m;
    vector<vector<int>> adj;
    vector<pair<int, int>> edges;

    bool maintain_dsu;
    DSU dsu;

    UndirectedGraph(int _n = 0, bool _maintain_dsu = false) {
        init(_n, _maintain_dsu);
    }

    void init(int _n, bool _maintain_dsu = false) {
        m = 0;
        n = _n;
        maintain_dsu = _maintain_dsu;
        adj.assign(n + 1, {});
        edges.clear();

        if(maintain_dsu) {
            dsu.init(n);
        }
    }

    void read_edges(int _m, int idx_offset = -1) {
        for(int i = 0; i < _m; i++) {
            int u, v;
            cin >> u >> v;
            u += idx_offset;
            v += idx_offset;
            add_edge(u, v);
        }
    }

    void add_edges(const vector<pair<int, int>>& new_edges) {
        for(auto [u, v]: new_edges) {
            add_edge(u, v);
        }
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        edges.emplace_back(u, v);
        m++;

        if(maintain_dsu) {
            dsu.unite(u, v);
        }
    }

    DSU& get_dsu() {
        if(!maintain_dsu) {
            dsu.init(n);
            for(auto [u, v]: edges) {
                dsu.unite(u, v);
            }
        }

        return dsu;
    }
};
