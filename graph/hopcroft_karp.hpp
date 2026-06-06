#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp algorithm for bipartite matching that runs in O(E sqrt(V)).
// Motivated by this submission: https://judge.yosupo.jp/submission/52112, but
// adapted to match coding_library/graph/bipartite_matching.cpp.

class HopcroftKarp {
  private:
    int n, m;
    vector<int> dist;

    bool bfs() {
        queue<int> q;
        dist.assign(n, -1);
        for(int u = 0; u < n; u++) {
            if(inv_match[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool found = false;
        while(!q.empty()) {
            int u = q.front();
            q.pop();
            for(int v: adj[u]) {
                int m = match[v];
                if(m == -1) {
                    found = true;
                } else if(dist[m] == -1) {
                    dist[m] = dist[u] + 1;
                    q.push(m);
                }
            }
        }

        return found;
    }

    bool dfs(int u) {
        for(int v: adj[u]) {
            int m = match[v];
            if(m == -1 || (dist[m] == dist[u] + 1 && dfs(m))) {
                inv_match[u] = v;
                match[v] = u;
                return true;
            }
        }
        dist[u] = -1;
        return false;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    HopcroftKarp(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    int max_matching(bool shuffle_edges = false) {
        if(shuffle_edges) {
            for(int i = 0; i < n; i++) {
                shuffle(
                    adj[i].begin(), adj[i].end(),
                    mt19937(
                        chrono::steady_clock::now().time_since_epoch().count()
                    )
                );
            }
        }

        int ans = 0;
        while(bfs()) {
            for(int u = 0; u < n; u++) {
                if(inv_match[u] == -1 && dfs(u)) {
                    ans++;
                }
            }
        }
        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> matches;
        for(int u = 0; u < n; u++) {
            if(inv_match[u] != -1) {
                matches.emplace_back(u, inv_match[u]);
            }
        }
        return matches;
    }

    pair<vector<int>, vector<int>> minimum_vertex_cover() {
        // König: alternating-BFS from unmatched left vertices marks the
        // reachable set Z; cover = (Left \ Z) on the left side together with
        // (Right in Z) on the right side. Here dist[u] != -1 means u is in Z.

        vector<int> left_cover, right_cover;
        bfs();

        for(int u = 0; u < n; u++) {
            if(dist[u] == -1) {
                left_cover.push_back(u);
            }
        }

        for(int v = 0; v < m; v++) {
            if(match[v] != -1 && dist[match[v]] != -1) {
                right_cover.push_back(v);
            }
        }

        return {left_cover, right_cover};
    }

    pair<vector<int>, vector<int>> maximum_independent_set() {
        // The complement of a minimum vertex cover is a maximum independent
        // set (König), so keep exactly the vertices not in the cover.

        auto [left_cover, right_cover] = minimum_vertex_cover();
        vector<char> in_left_cover(n), in_right_cover(m);
        for(int u: left_cover) {
            in_left_cover[u] = 1;
        }

        for(int v: right_cover) {
            in_right_cover[v] = 1;
        }

        vector<int> left_set, right_set;
        for(int u = 0; u < n; u++) {
            if(!in_left_cover[u]) {
                left_set.push_back(u);
            }
        }

        for(int v = 0; v < m; v++) {
            if(!in_right_cover[v]) {
                right_set.push_back(v);
            }
        }

        return {left_set, right_set};
    }
};

using BipartiteMatching = HopcroftKarp;
