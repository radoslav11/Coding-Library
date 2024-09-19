#include <bits/stdc++.h>
#include <coding_library/graph/dsu.hpp>
#include <coding_library/tree/lca_sparse_table.hpp>
using namespace std;

class LineTree {
  private:
    vector<vector<int>> adj;
    LCAUtilsRMQ helper_lca;
    vector<int> vals;

    dsu uf;
    vector<int> best_node;

  public:
    LineTree() {}
    LineTree(
        vector<vector<int>> _adj, vector<int> _vals, bool one_indexed = true
    ) {
        init(_adj, _vals, one_indexed);
    }

    void init(
        vector<vector<int>> _adj, vector<int> _vals, bool one_indexed = true
    ) {
        adj = _adj;
        vals = _vals;
        int n = (int)vals.size() - (int)one_indexed;
        helper_lca.init(n);
        uf.init(n);

        vector<int> perm(n);
        iota(perm.begin(), perm.end(), (int)one_indexed);
        sort(perm.begin(), perm.end(), [&](int u, int v) {
            return pair<int, int>{vals[u], u} > pair<int, int>{vals[v], v};
        });

        best_node.resize(n + 1);
        for(int i = 1; i <= n; i++) {
            best_node[i] = i;
        }

        for(int u: perm) {
            for(int v: adj[u]) {
                if(pair<int, int>{vals[v], v} > pair<int, int>{vals[u], u}) {
                    helper_lca.add_edge(u, best_node[uf.root(v)]);
                    uf.unite(u, v);
                }
            }

            best_node[uf.root(u)] = u;
        }

        helper_lca.prepare(perm.back());
    }

    int min_value(int u, int v) { return vals[helper_lca.lca(u, v)]; }

    int min_node(int u, int v) { return helper_lca.lca(u, v); }
};
