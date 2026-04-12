#include <bits/stdc++.h>
using namespace std;

class STNumbering {
  private:
    vector<int> low, disc, par, sign;
    vector<int> preorder;
    int dfs_time;

    bool tarjan_check(int u, int pr) {
        low[u] = disc[u] = ++dfs_time;
        int child_cnt = 0;
        for(int v: adj[u]) {
            if(v != pr) {
                if(disc[v] == -1) {
                    child_cnt++;
                    if(!tarjan_check(v, u)) {
                        return false;
                    }
                    low[u] = min(low[u], low[v]);
                    if(pr != -1 && low[v] >= disc[u]) {
                        return false;
                    }
                } else {
                    low[u] = min(low[u], disc[v]);
                }
            }
        }
        if(pr == -1 && child_cnt > 1) {
            return false;
        }
        return true;
    }

    void tarjan(int u, int pr) {
        low[u] = disc[u] = ++dfs_time;
        for(int v: adj[u]) {
            if(v != pr) {
                if(disc[v] == -1) {
                    preorder.push_back(v);
                    tarjan(v, u);
                    low[u] = min(low[u], low[v]);
                    par[v] = u;
                } else {
                    low[u] = min(low[u], disc[v]);
                }
            }
        }
    }

  public:
    int n;
    vector<vector<int>> adj;
    vector<int> ordering;

    STNumbering() {}
    STNumbering(int _n) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n, {});
        ordering.clear();
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    bool find_ordering(int s, int t) {
        low.assign(n, -1);
        disc.assign(n, -1);
        par.assign(n, -1);
        sign.assign(n, 0);
        preorder.clear();
        ordering.clear();
        dfs_time = 0;

        adj[s].push_back(t);
        adj[t].push_back(s);

        if(!tarjan_check(t, -1)) {
            adj[s].pop_back();
            adj[t].pop_back();
            return false;
        }

        for(int i = 0; i < n; i++) {
            if(disc[i] == -1) {
                adj[s].pop_back();
                adj[t].pop_back();
                return false;
            }
        }

        fill(low.begin(), low.end(), -1);
        fill(disc.begin(), disc.end(), -1);
        fill(sign.begin(), sign.end(), 0);
        preorder.clear();
        dfs_time = 0;

        disc[s] = low[s] = ++dfs_time;
        sign[disc[s]] = -1;
        tarjan(t, -1);

        list<int> st_li;
        vector<list<int>::iterator> it_ver(n + 1);

        st_li.push_back(s);
        st_li.push_back(t);
        it_ver[disc[s]] = st_li.begin();
        it_ver[disc[t]] = next(st_li.begin());

        for(int v: preorder) {
            if(sign[low[v]] == -1) {
                it_ver[disc[v]] = st_li.insert(it_ver[disc[par[v]]], v);
            } else {
                it_ver[disc[v]] = st_li.insert(next(it_ver[disc[par[v]]]), v);
            }
            sign[disc[par[v]]] = -sign[low[v]];
        }

        ordering.assign(st_li.begin(), st_li.end());
        adj[s].pop_back();
        adj[t].pop_back();
        return true;
    }
};
