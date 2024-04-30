#include <bits/stdc++.h>
using namespace std;

template<class FlowT>
struct max_flow {
    const static FlowT finf = 1e18 + 42 + 17;
    const static FlowT feps = 0;

    struct edge {
        FlowT flow, cap;
        int idx, rev, to;
        edge() {
            flow = 0;
            cap = 0;
            rev = 0;
            idx = 0;
            to = 0;
        }
        edge(int _to, int _rev, FlowT _flow, FlowT _cap, int _idx) {
            to = _to;
            rev = _rev;
            flow = _flow;
            cap = _cap;
            idx = _idx;
        }
    };

    vector<vector<edge>> G;
    vector<int> dist, po;
    int n;

    bool bfs(int s, int t) {
        dist[s] = -1, po[s] = 0;
        dist[t] = -1, po[t] = 0;
        for(int v = 0; v < n; v++) {
            dist[v] = -1, po[v] = 0;
        }

        queue<int> Q;
        Q.push(s);
        dist[s] = 0;

        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for(edge e: G[u]) {
                if(dist[e.to] == -1 && e.flow < e.cap) {
                    dist[e.to] = dist[u] + 1;
                    Q.push(e.to);
                }
            }
        }

        return dist[t] != -1;
    }

    FlowT dfs(int u, int t, FlowT fl = finf) {
        if(u == t) {
            return fl;
        }

        for(; po[u] < G[u].size(); po[u]++) {
            auto &e = G[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap) {
                FlowT f = dfs(e.to, t, min(fl, e.cap - e.flow));
                e.flow += f;
                G[e.to][e.rev].flow -= f;
                if(f > 0) {
                    return f;
                }
            }
        }

        return 0;
    }

    void init(int _n) {
        n = _n;
        G.assign(n + 1, {});
        dist.resize(n + 1);
        po.resize(n + 1);
    }

    void add_edge(int u, int v, FlowT w, int idx = -1) {
        G[u].push_back(edge(v, G[v].size(), 0, w, idx));
        G[v].push_back(edge(u, G[u].size() - 1, 0, 0, -1));
    }

    FlowT flow(int s, int t) {
        if(s == t) {
            return finf;
        }
        FlowT ret = 0, to_add;
        while(bfs(s, t)) {
            while((to_add = dfs(s, t))) {
                ret += to_add;
            }
        }

        return ret;
    }
};

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
