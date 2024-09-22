#include <bits/stdc++.h>
using namespace std;

template<class T>
class MaxFlow {
  private:
    const static T INF = numeric_limits<T>::max();

    struct Edge {
        T flow, cap;
        int idx, rev, to;
        Edge(int _to, int _rev, T _flow, T _cap, int _idx)
            : to(_to), rev(_rev), flow(_flow), cap(_cap), idx(_idx) {}
    };

    vector<int> dist, po;
    int n;

    bool bfs(int s, int t) {
        fill(dist.begin(), dist.end(), -1);
        fill(po.begin(), po.end(), 0);

        queue<int> q;
        q.push(s);
        dist[s] = 0;

        while(!q.empty()) {
            int u = q.front();
            q.pop();

            for(Edge e: adj[u]) {
                if(dist[e.to] == -1 && e.flow < e.cap) {
                    dist[e.to] = dist[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return dist[t] != -1;
    }

    T dfs(int u, int t, T fl = INF) {
        if(u == t) {
            return fl;
        }

        for(; po[u] < (int)adj[u].size(); po[u]++) {
            auto &e = adj[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap) {
                T f = dfs(e.to, t, min(fl, e.cap - e.flow));
                e.flow += f;
                adj[e.to][e.rev].flow -= f;
                if(f > 0) {
                    return f;
                }
            }
        }

        return 0;
    }

  public:
    MaxFlow(int n = 0) {
        init(n);
    }

    vector<vector<Edge>> adj;

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
        dist.resize(n + 1);
        po.resize(n + 1);
    }

    void add_edge(int u, int v, T w, int idx = -1) {
        adj[u].push_back(Edge(v, adj[v].size(), 0, w, idx));
        adj[v].push_back(Edge(u, adj[u].size() - 1, 0, 0, -1));
    }

    T flow(int s, int t) {
        assert(s != t);

        T ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
            }
        }

        return ret;
    }
};
