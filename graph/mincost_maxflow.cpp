#include <bits/stdc++.h>

const int INFi = 1e9;
const int64_t INF = 2e18;

// From a submission by Ormlis, with tiny edits
// that I like.

template<typename T>
struct min_cost_flow {
    struct edge {
        int to, rev;
        T capacity, cost, flow;
    };

    int n;
    std::vector<std::vector<edge>> g;

    min_cost_flow(int n = 0) : n(n), g(n) {}

    void init(int _n) {
        n = _n;
        g.assign(n, {});
    }

    int size() const { return n; }

    int add_edge(int from, int to, T capacity, T cost) {
        int id = g[from].size();
        g[from].push_back(
            {to, int(g[to].size()) + (from == to), capacity, cost, 0}
        );
        g[to].push_back({from, id, 0, -cost, 0});
        return id;
    }

    std::pair<T, T> flow(
        int source, int sink, T flow_limit = std::numeric_limits<T>::max()
    ) {
        for(int v = 0; v < n; v++) {
            for(edge &e: g[v]) {
                e.flow = 0;
            }
        }

        T cost = 0, flow = 0;
        static constexpr T INF = std::numeric_limits<T>::max();
        std::vector<T> potential(n);

        auto build_potential = [&]() {
            std::fill(potential.begin(), potential.end(), INF);
            potential[source] = 0;

            while(true) {
                bool any = false;
                for(int v = 0; v < n; v++) {
                    for(const auto &[u, rev, capacity, cost, flow]: g[v]) {
                        if(capacity != 0 && potential[v] != INF &&
                           potential[v] + cost < potential[u]) {
                            potential[u] = potential[v] + cost;
                            any = true;
                        }
                    }
                }

                if(!any) {
                    break;
                }
            }
        };

        build_potential();

        std::vector<std::pair<int, int>> parent(n);
        std::vector<std::pair<T, T>> dist(n);

        auto dijkstra = [&]() {
            std::fill(
                dist.begin(), dist.end(),
                std::pair<T, T>{INF, flow_limit - flow}
            );
            dist[source].first = 0;

            std::priority_queue<
                std::pair<T, int>, std::vector<std::pair<T, int>>,
                std::greater<>>
                q;
            q.push({0, source});

            while(q.size()) {
                const auto [cur_dist, v] = q.top();
                q.pop();
                if(cur_dist > dist[v].first) {
                    continue;
                }

                for(const auto &[u, rev, capacity, cost, flow]: g[v]) {
                    if(potential[u] != INF &&
                       cur_dist + cost - potential[u] + potential[v] <
                           dist[u].first &&
                       flow < capacity) {
                        parent[u] = {v, rev};
                        dist[u] = {
                            cur_dist + cost - potential[u] + potential[v],
                            std::min(dist[v].second, capacity - flow)
                        };
                        q.push({dist[u].first, u});
                    }
                }
            }
            return dist[sink].first != INF;
        };

        while(flow < flow_limit && dijkstra()) {
            T delta = dist[sink].second;
            flow += delta;

            for(int v = sink; v != source; v = parent[v].first) {
                cost -= g[v][parent[v].second].cost * delta;
                g[v][parent[v].second].flow -= delta;
                g[parent[v].first][g[v][parent[v].second].rev].flow += delta;
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
