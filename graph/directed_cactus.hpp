#include <bits/stdc++.h>
using namespace std;

class DirectedCactus {
  private:
    int n;
    vector<vector<int>> adj;
    vector<int> nxt, jump, dep, in_time, out_time;

    void dfs(int u, int &dfs_time, vector<bool> &used, vector<int> &buff) {
        used[u] = true;
        in_time[u] = dfs_time++;
        buff.push_back(u);
        for(int v: adj[u]) {
            if(!used[v]) {
                dep[v] = dep[u] + 1;
                dfs(v, dfs_time, used, buff);
            }
        }
        out_time[u] = dfs_time - 1;
    }

  public:
    DirectedCactus() {
        n = 0;
        nxt.clear();
    }
    DirectedCactus(vector<int> _nxt) { init(_nxt); }

    void init(vector<int> _nxt) {
        nxt = _nxt;
        n = nxt.size();
        in_time.resize(n);
        out_time.resize(n);
        jump.resize(n);

        adj.assign(n, {});
        for(int i = 0; i < n; i++) {
            adj[nxt[i]].push_back(i);
        }

        int dfs_time = 0;
        vector<bool> used(n, false);
        dep.assign(n, 0);
        for(int i = 0; i < n; i++) {
            if(!used[i]) {
                vector<int> buff;

                int root = i;
                while(!used[root]) {
                    used[root] = true;
                    buff.push_back(root);
                    root = nxt[root];
                }

                for(int u: buff) {
                    used[u] = false;
                }
                buff.clear();

                dfs(root, dfs_time, used, buff);
                for(int u: buff) {
                    jump[u] = nxt[root];
                }
            }
        }
    }

    int distance(int from, int to) {
        if(in_time[to] <= in_time[from] && out_time[from] <= out_time[to]) {
            return dep[from] - dep[to];
        }

        int to_add = dep[from] + 1;
        from = jump[from];
        if(in_time[to] <= in_time[from] && out_time[from] <= out_time[to]) {
            return dep[from] - dep[to] + to_add;
        }
        return -1;
    }
};
