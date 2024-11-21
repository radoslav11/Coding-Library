#include <bits/stdc++.h>
using namespace std;

// This implementation find cnt_odd_vertices / 2 eulerian paths that cover
// all edges of the graph. The way to use it is to create an object and
// incrementally add edges to it. Then, call find_paths() to get the paths.
// If the number of paths is 1, we have either a cycle or a path.
//
// Tested on:
// https://codeforces.com/problemsets/acmsguru/problem/99999/101
// https://codesprintla24.kattis.com/contests/codesprintla24open/problems/catbusplan

class EulerianPaths {
  private:
    void dfs(int u, vector<int>& path, vector<bool>& used, vector<int>& po) {
        for(; po[u] < (int)adj[u].size();) {
            int idx = po[u]++;
            if(!used[adj[u][idx].second >> 1]) {
                used[adj[u][idx].second >> 1] = true;
                dfs(adj[u][idx].first, path, used, po);
                path.push_back(adj[u][idx].second);
            }
        }
    }

  public:
    int n, m;
    vector<int> deg;
    vector<vector<pair<int, int>>> adj;
    vector<pair<int, int>> edges;

    EulerianPaths(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        m = 0;
        adj.assign(n + 1, {});
        deg.assign(n + 1, 0);
        edges.clear();
    }

    int add_edge(int u, int v) {
        adj[u].push_back({v, m * 2});
        adj[v].push_back({u, m * 2 + 1});
        edges.push_back({u, v});
        deg[u]++;
        deg[v]++;
        m++;

        return edges.size() - 1;
    }

    vector<vector<int>> find_paths() {
        vector<bool> used(m, false);
        vector<int> po(n + 1, 0);

        vector<int> odd_vertices;
        for(int i = 0; i <= n; i++) {
            if(deg[i] % 2 == 1) {
                odd_vertices.push_back(i);
            }
        }

        int total_edges = m;
        for(int i = 0; i < (int)odd_vertices.size() / 2; i++) {
            int u = odd_vertices[2 * i], v = odd_vertices[2 * i + 1];
            adj[u].push_back({v, 2 * total_edges});
            adj[v].push_back({u, 2 * total_edges + 1});
            total_edges++;
            used.push_back(false);
            edges.push_back({u, v});
        }

        vector<vector<int>> paths;
        for(int u = 0; u <= n; u++) {
            if(!adj[u].empty()) {
                vector<int> path;
                dfs(u, path, used, po);
                if(!path.empty()) {
                    // Rotate the path so that we always start with a fake edge
                    // if there is at least one.
                    auto it = find_if(path.begin(), path.end(), [&](int x) {
                        return x >= 2 * m;
                    });
                    if(it != path.end()) {
                        rotate(path.begin(), it, path.end());
                    }

                    vector<int> current_path;
                    for(int x: path) {
                        if(x < 2 * m) {
                            current_path.push_back(x);
                        } else if(!current_path.empty()) {
                            paths.push_back(current_path);
                            current_path.clear();
                        }
                    }
                    if(!current_path.empty()) {
                        paths.push_back(current_path);
                    }
                }
            }
        }

        return paths;
    }

    pair<int, int> get_edge(int edge_i) {
        if(edge_i & 1) {
            return edges[edge_i >> 1];
        } else {
            return {edges[edge_i >> 1].second, edges[edge_i >> 1].first};
        }
    }

    vector<pair<int, int>> get_path_edges(const vector<int>& path) {
        vector<pair<int, int>> result;
        for(int edge_i: path) {
            result.push_back(get_edge(edge_i));
        }
        return result;
    }

    bool is_cycle(const vector<int>& path) {
        int start = get_edge(path[0]).first;
        int end = get_edge(path.back()).second;
        return start == end;
    }
};
