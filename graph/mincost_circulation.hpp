#include <bits/stdc++.h>
using namespace std;

// In some rare cases, we might want to find the min cost circulation instead of
// min cost flow, and this is non-trivial in the presence of negative cycles (as
// we would like to select them up to their capacity). The two common approaches are 
// using cost scaling push relabel, or the network simplex. Some resources to check are:
//
//     - https://codeforces.com/blog/entry/104075?#comment-925064
//     - https://codeforces.com/blog/entry/57018
//     - https://codeforces.com/blog/entry/94190 (+ regular simplex)
//
// Here we use bicsi's implementation, based on brunovsky's blog (last one mentioned), which
// is fairly clean and works well (and it's easy to recover the answer).

template<typename T>
class MinCostCirculation {
  private:
    struct Edge {
        int from, to;
        T capacity, cost, flow;
        Edge(int _from, int _to, T _capacity, T _cost, T _flow = 0)
            : from(_from),
              to(_to),
              capacity(_capacity),
              cost(_cost),
              flow(_flow) {}
    };

    int n;
    vector<Edge> edges;
    vector<int> pei, depth;
    vector<T> dual;
    vector<set<int>> tree;

    void dfs(int node) {
        for(auto ei: tree[node]) {
            if(ei == pei[node]) {
                continue;
            }
            int vec = edges[ei].to;
            dual[vec] = dual[node] + edges[ei].cost;
            pei[vec] = (ei ^ 1);
            depth[vec] = 1 + depth[node];
            dfs(vec);
        }
    }

    template<typename CB>
    void walk(int ei, CB&& cb) {
        cb(ei);
        int a = edges[ei].from, b = edges[ei].to;
        while(a != b) {
            if(depth[a] > depth[b]) {
                cb(pei[a] ^ 1), a = edges[pei[a]].to;
            } else {
                cb(pei[b]), b = edges[pei[b]].to;
            }
        }
    }

  public:
    MinCostCirculation(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        edges.clear();
        pei.assign(n + 1, -1);
        depth.assign(n + 1, 0);
        dual.assign(n + 1, 0);
        tree.assign(n + 1, set<int>());
    }

    int size() const { return n; }

    int add_edge(int from, int to, T capacity, T cost) {
        int id = edges.size();
        edges.push_back(Edge(from, to, capacity, cost, 0));
        edges.push_back(Edge(to, from, 0, -cost, 0));
        return id;
    }

    T min_circulation() {
        for(int i = 0; i < n; i++) {
            int ei = add_edge(n, i, 0, 0);
            tree[n].insert(ei);
            tree[i].insert(ei ^ 1);
        }

        T answer = 0;
        T flow;
        int cost, ein, eout, ptr = 0;
        const int B = 3 * n;
        for(int z = 0; z < (int)edges.size() / B + 1; z++) {
            if(!z) {
                dfs(n);
            }

            pair<T, int> pin = {0, -1};
            for(int t = 0; t < B; t++, (++ptr) %= (int)edges.size()) {
                auto& e = edges[ptr];
                if(e.flow < e.capacity) {
                    pin =
                        min(pin,
                            make_pair(dual[e.from] + e.cost - dual[e.to], ptr));
                }
            }

            tie(cost, ein) = pin;
            if(cost == 0) {
                continue;
            }

            pair<T, int> pout = {edges[ein].capacity - edges[ein].flow, ein};
            walk(ein, [&](int ei) {
                pout =
                    min(pout,
                        make_pair(edges[ei].capacity - edges[ei].flow, ei));
            });

            tie(flow, eout) = pout;
            walk(ein, [&](int ei) {
                edges[ei].flow += flow, edges[ei ^ 1].flow -= flow;
            });

            tree[edges[ein].from].insert(ein);
            tree[edges[ein].to].insert(ein ^ 1);
            tree[edges[eout].from].erase(eout);
            tree[edges[eout].to].erase(eout ^ 1);

            answer += flow * cost;
            z = -1;
        }
        return answer;
    }

    const Edge& get_edge(int id) const { return edges[id]; }
};
