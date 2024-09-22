#include <bits/stdc++.h>
using namespace std;

class BipartiteMatching {
  private:
    int n, m, visited_timer;
    vector<int> visited;

    bool try_kuhn(int u) {
        if(visited[u] == visited_timer) {
            return false;
        }

        visited[u] = visited_timer;
        for(int v: adj[u]) {
            if(match[v] == -1 || try_kuhn(match[v])) {
                match[v] = u;
                inv_match[u] = v;
                return true;
            }
        }
        return false;
    }

    int pre_match() {
        int matching_size = 0;
        for(int u = 0; u < n; u++) {
            if(inv_match[u] == -1) {
                for(int v: adj[u]) {
                    if(match[v] == -1) {
                        matching_size++;
                        match[v] = u;
                        inv_match[u] = v;
                        break;
                    }
                }
            }
        }

        return matching_size;
    }

  public:
    vector<int> match, inv_match;
    vector<vector<int>> adj;

    BipartiteMatching(int _n, int _m = -1) : n(_n), m(_m == -1 ? _n : _m) {
        adj.assign(n, vector<int>());
        clear(false);
    }

    void clear(bool clear_adj = true) {
        match.assign(m, -1);
        inv_match.assign(n, -1);

        visited_timer = 0;
        visited.assign(n, 0);
        if(clear_adj) {
            adj.assign(n, vector<int>());
        }
    }

    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool match_vertex(int u) {
        if(inv_match[u] != -1) {
            return false;
        }

        visited_timer++;
        return try_kuhn(u);
    }

    int max_matching(bool shuffle_edges = false, bool pre_matching = false) {
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
        if(pre_matching) {
            ans += pre_match();
        }

        for(int i = 0; i < n; i++) {
            ans += match_vertex(i);
        }

        return ans;
    }

    vector<pair<int, int>> get_matching() {
        vector<pair<int, int>> res;
        for(int i = 0; i < m; i++) {
            if(match[i] != -1) {
                res.emplace_back(match[i], i);
            }
        }
        return res;
    }
};
