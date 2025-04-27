#include <bits/stdc++.h>
#include <coding_library/data_structures/heap.hpp>

using namespace std;

template<typename T>
vector<T> dijkstra(int src, const vector<vector<pair<int, T>>>& adj) {
    static auto cmp_min = [](int a, int b) -> bool { return a > b; };

    int n = adj.size();
    vector<T> dist(n, numeric_limits<T>::max());
    vector<int> pq_node(n, -1);
    vector<int> pq_node_to_ver(n, -1);
    Heap<T, +cmp_min> pq;

    function<void(int, T)> update = [&](int u, T d) {
        dist[u] = min(dist[u], d);
        if(pq_node[u] == -1) {
            pq_node[u] = pq.push(dist[u]);
            pq_node_to_ver[pq_node[u]] = u;
        } else {
            pq.update(pq_node[u], dist[u]);
        }
    };

    update(src, 0);
    while(!pq.empty()) {
        int u = pq_node_to_ver[pq.top_node()];
        pq.pop();

        for(auto [v, w]: adj[u]) {
            update(v, dist[u] + w);
        }
    }

    return dist;
}
