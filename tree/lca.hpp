#include <bits/stdc++.h>
using namespace std;

class LCAUtils {
  protected:
    int max_log, dfs_time;
    vector<vector<int>> par_up;

    void dfs_lca(int u, int pr, int &dfs_time) {
        in_time[u] = ++dfs_time;
        par_up[u][0] = pr;
        for(int i = 1; i < max_log; i++) {
            par_up[u][i] = par_up[par_up[u][i - 1]][i - 1];
        }

        for(int v: adj[u]) {
            if(v != pr) {
                dfs_lca(v, u, dfs_time);
            }
        }

        out_time[u] = ++dfs_time;
    }

  public:
    int n;
    vector<int> in_time, out_time;
    vector<vector<int>> adj;

    LCAUtils() {}

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void prepare(int root = 1) {
        max_log = 1;
        while((1 << max_log) <= n) {
            max_log++;
        }

        par_up.assign(n + 1, vector<int>(max_log));

        in_time.resize(n + 1);
        out_time.resize(n + 1);
        dfs_time = 0;
        dfs_lca(root, root, dfs_time);
    }

    int lca(int u, int v) {
        if(upper(u, v)) {
            return u;
        }
        if(upper(v, u)) {
            return v;
        }

        int a = u;
        for(int i = max_log - 1; i >= 0; i--) {
            if(!upper(par_up[a][i], v)) {
                a = par_up[a][i];
            }
        }

        return par_up[a][0];
    }

    inline bool upper(int u, int v) {
        return in_time[u] <= in_time[v] && out_time[v] <= out_time[u];
    }
};
