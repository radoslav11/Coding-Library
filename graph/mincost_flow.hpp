#include <bits/stdc++.h>
using namespace std;

// Based on Ormlis' implementation

template<typename T>
class MinCostFlow {
  private:
    const static T INF = numeric_limits<T>::max();

    struct Edge {
        int to, rev;
        T capacity, cost, flow;
        Edge(int _to, int _rev, T _capacity, T _cost, T _flow)
            : to(_to),
              rev(_rev),
              capacity(_capacity),
              cost(_cost),
              flow(_flow) {}
    };

    int n;
    vector<vector<Edge>> g;
    vector<T> potential;
    vector<pair<int, int>> parent;
    vector<pair<T, T>> dist;

    void build_potential(int source) {
        fill(potential.begin(), potential.end(), INF);
        potential[source] = 0;

        while(true) {
            bool any = false;
            for(int v = 0; v < n; v++) {
                for(const auto& e: g[v]) {
                    if(e.capacity != 0 && potential[v] != INF &&
                       potential[v] + e.cost < potential[e.to]) {
                        potential[e.to] = potential[v] + e.cost;
                        any = true;
                    }
                }
            }
            if(!any) {
                break;
            }
        }
    }

    bool dijkstra(int source, int sink, T flow_limit, T flow) {
        fill(dist.begin(), dist.end(), make_pair(INF, flow_limit - flow));
        dist[source].first = 0;

        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> q;
        q.push({0, source});

        while(!q.empty()) {
            auto [cur_dist, v] = q.top();
            q.pop();
            if(cur_dist > dist[v].first) {
                continue;
            }

            for(const auto& e: g[v]) {
                if(potential[e.to] != INF &&
                   cur_dist + e.cost - potential[e.to] + potential[v] <
                       dist[e.to].first &&
                   e.flow < e.capacity) {
                    parent[e.to] = {v, e.rev};
                    dist[e.to] = {
                        cur_dist + e.cost - potential[e.to] + potential[v],
                        min(dist[v].second, e.capacity - e.flow)
                    };
                    q.push({dist[e.to].first, e.to});
                }
            }
        }
        return dist[sink].first != INF;
    }

  public:
    MinCostFlow(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        g.assign(n, {});
        potential.resize(n);
        parent.resize(n);
        dist.resize(n);
    }

    int size() const { return n; }

    int add_edge(int from, int to, T capacity, T cost) {
        int id = g[from].size();
        g[from].push_back(
            Edge(to, g[to].size() + (from == to), capacity, cost, 0)
        );
        g[to].push_back(Edge(from, id, 0, -cost, 0));
        return id;
    }

    pair<T, T> flow(int source, int sink, T flow_limit = INF) {
        for(int v = 0; v < n; v++) {
            for(auto& e: g[v]) {
                e.flow = 0;
            }
        }

        T cost = 0, flow = 0;
        build_potential(source);

        while(flow < flow_limit && dijkstra(source, sink, flow_limit, flow)) {
            T delta = dist[sink].second;
            flow += delta;

            for(int v = sink; v != source; v = parent[v].first) {
                auto& e = g[parent[v].first][g[v][parent[v].second].rev];
                cost += e.cost * delta;
                e.flow += delta;
                g[v][parent[v].second].flow -= delta;
            }

            for(int i = 0; i < n; i++) {
                if(dist[i].first != INF) {
                    potential[i] += dist[i].first;
                }
            }
        }
        return {cost, flow};
    }
};
