#include <bits/stdc++.h>
#include <coding_library/data_structures/sparse_table.hpp>
using namespace std;

class LCAUtilsRMQ {
  private:
    static pair<int, int> _min_custom(pair<int, int> a, pair<int, int> b) {
        return min(a, b);
    }

    SparseTable<pair<int, int>, _min_custom> rmq;
    vector<int> pos, dep;
    vector<pair<int, int>> order;

    void pre_dfs(int u, int pr = -1, int d = 0) {
        pos[u] = order.size();
        dep[u] = d;
        order.push_back({d, u});

        for(int v: adj[u]) {
            if(v != pr) {
                pre_dfs(v, u, d + 1);
                order.push_back({d, u});
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj;

    LCAUtilsRMQ() { n = 0; }
    LCAUtilsRMQ(int _n) { init(_n); }
    LCAUtilsRMQ(int _n, const vector<vector<int>>& _adj) {
        init(_n);
        adj = _adj;
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void init(int _n) {
        n = _n;
        order.clear();
        adj.assign(n, {});
    }

    void prepare(int root = 0) {
        order.clear();
        pos.resize(n);
        dep.resize(n);
        pre_dfs(root);
        rmq.init(order);
    }

    int lca(int u, int v) {
        if(pos[u] > pos[v]) {
            swap(u, v);
        }
        return rmq.query(pos[u], pos[v]).second;
    }

    int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
};
