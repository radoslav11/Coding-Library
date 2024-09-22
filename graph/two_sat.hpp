#include <bits/stdc++.h>
using namespace std;

class TwoSat {
  private:
    vector<bool> visited;

    void dfs1(int u) {
        visited[u] = true;
        for(int v: adj[u]) {
            if(!visited[v]) {
                dfs1(v);
            }
        }

        top_sort.push_back(u);
    }

    void dfs2(int u) {
        for(int v: radj[u]) {
            if(comp[v] == -1) {
                comp[v] = comp[u];
                dfs2(v);
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj, radj;
    vector<int> comp, comp_ids, top_sort;

    TwoSat() {}
    TwoSat(int _n) { init(_n); }

    void init(int _n) {
        n = _n;
        comp_ids.clear();
        top_sort.clear();
        adj.assign(2 * n, {});
        radj.assign(2 * n, {});
    }

    void add_implication(int u, int v, bool neg_u = false, bool neg_v = false) {
        adj[u << 1 | neg_u].push_back(v << 1 | neg_v);
        radj[v << 1 | neg_v].push_back(u << 1 | neg_u);
    }

    pair<bool, vector<bool>> solve() {
        comp.assign(2 * n, -1);
        visited.assign(2 * n, false);

        for(int i = 0; i < 2 * n; i++) {
            if(!visited[i]) {
                dfs1(i);
            }
        }

        reverse(top_sort.begin(), top_sort.end());
        for(int u: top_sort) {
            if(comp[u] == -1) {
                comp[u] = (int)comp_ids.size();
                comp_ids.push_back(comp[u]);
                dfs2(u);
            }
        }

        vector<bool> assignment(n);
        for(int i = 0; i < n; i++) {
            if(comp[i << 1] == comp[i << 1 | 1]) {
                return {false, {}};
            }

            assignment[i] = comp[i << 1] > comp[i << 1 | 1];
        }

        return {true, assignment};
    }
};
