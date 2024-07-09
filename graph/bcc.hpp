#include <bits/stdc++.h>

using namespace std;

class biconnected_components {
  private:
    vector<int> low, disc;

    void tarjan(int u, int pr, int &dfs_time, vector<pair<int, int>> &st) {
        low[u] = disc[u] = ++dfs_time;

        int child_cnt = 0;
        for(int v: adj[u]) {
            if(v != pr) {
                if(disc[v] == -1) {
                    st.push_back({u, v});
                    tarjan(v, u, dfs_time, st);
                    child_cnt++;
                    low[u] = min(low[u], low[v]);

                    if((pr == -1 && child_cnt > 1) ||
                       (pr != -1 && disc[u] <= low[v])) {
                        vector<pair<int, int>> curr = {make_pair(u, v)};
                        while(st.back() != make_pair(u, v)) {
                            curr.push_back(st.back());
                            st.pop_back();
                        }

                        st.pop_back();
                        if(curr.size() == 1) {
                            bridges.push_back(curr.back());
                        } else {
                            bcc.push_back(curr);
                        }
                    }
                } else if(disc[v] < disc[u]) {
                    low[u] = min(low[u], disc[v]);
                    st.push_back({u, v});
                }
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj;
    vector<vector<pair<int, int>>> bcc;
    vector<pair<int, int>> bridges;

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
    }

    void compute_bcc() {
        bcc.clear();
        bridges.clear();
        low.assign(n + 1, -1);
        disc.assign(n + 1, -1);

        int dfs_time = 0;
        for(int i = 0; i < n; i++) {
            if(disc[i] == -1) {
                vector<pair<int, int>> st;
                tarjan(i, -1, dfs_time, st);
                if(!st.empty()) {
                    if(st.size() == 1) {
                        bridges.push_back(st.back());
                    } else {
                        bcc.push_back(st);
                    }
                    st.clear();
                }
            }
        }
    }
};

// int n, m;
// biconnected_components bcc;
// void read() {
//     cin >> n >> m;
//     bcc.init(n);
//     for(int i = 0; i < m; i++) {
//         int u, v;
//         cin >> u >> v;
//         bcc.add_edge(u, v);
//     }
// }
