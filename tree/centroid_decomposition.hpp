#include <bits/stdc++.h>
using namespace std;

template<
    void (*process)(int, const vector<vector<int>> &, const vector<bool> &)>
class CentroidDecomposition {
  private:
    int n;
    vector<vector<int>> adj;
    vector<bool> blocked;
    vector<int> subtree_size;

    int dfs(int u, int pr) {
        subtree_size[u] = 1;
        for(int v: adj[u]) {
            if(v != pr && !blocked[v]) {
                subtree_size[u] += dfs(v, u);
            }
        }
        return subtree_size[u];
    }

    int find_centroid(int u, int pr, int n) {
        for(int v: adj[u]) {
            if(v != pr && !blocked[v] && subtree_size[v] > n / 2) {
                return find_centroid(v, u, n);
            }
        }
        return u;
    }

  public:
    vector<int> link, centroid_depth;

    CentroidDecomposition(vector<vector<int>> adj = {}, int root = 0)
        : adj(adj) {
        n = adj.size();
        if(n > 0) {
            blocked.assign(n, false);
            link.assign(n, -1);
            centroid_depth.assign(n, -1);
            subtree_size.assign(n, 0);
            decompose(root);
        }
    }

    void decompose(int u, int prev_cen = -1, int depth = 0) {
        int n_here = dfs(u, -1);
        int cen = find_centroid(u, -1, n_here);

        link[cen] = prev_cen;
        centroid_depth[cen] = depth;
        process(cen, adj, blocked);

        blocked[cen] = true;
        for(int v: adj[cen]) {
            if(!blocked[v]) {
                decompose(v, cen, depth + 1);
            }
        }
        blocked[cen] = false;
    }
};
 
// void blank_process(int, const vector<vector<int>> &, const vector<bool> &) {}
// #include <coding_library/tree/lca_sparse_table.hpp>

// LCAUtilsRMQ tr;
// tr.init(n);
// for(int i = 1; i < n; i++) {
//     tr.add_edge(i, p[i]);
// }
// tr.prepare(0);

// Maintain closest active node query.

// CentroidDecomposition<blank_process> cd(tr.adj);
// vector<priority_queue<pair<int, int>>> best(n), to_remove(n);

// function<int(int)> closest_active = [&](int u) {
//     int cen_root = u, ans_dist = -1, ans_node = -1;
//     while(cen_root != -1) {
//         while(!best[cen_root].empty() && !to_remove[cen_root].empty() &&
//               to_remove[cen_root].top() == best[cen_root].top()) {
//             to_remove[cen_root].pop();
//             best[cen_root].pop();
//         }
//         if(!best[cen_root].empty()) {
//             int cand = best[cen_root].top().second;
//             int d = tr.dist(u, cand);
//             if(ans_dist == -1 || d < ans_dist) {
//                 ans_dist = d;
//                 ans_node = cand;
//             }
//         }
//         cen_root = cd.link[cen_root];
//     }
//     assert(ans_node != -1);
//     return ans_node;
// };

// function<void(int)> activate = [&](int u) {
//     int cen_root = u;
//     while(cen_root != -1) {
//         int d = tr.dist(u, cen_root);
//         best[cen_root].push({-d, u});
//         cen_root = cd.link[cen_root];
//     }
// };

// function<void(int)> deactivate = [&](int u) {
//     int cen_root = u;
//     while(cen_root != -1) {
//         int d = tr.dist(u, cen_root);
//         to_remove[cen_root].push({-d, u});
//         cen_root = cd.link[cen_root];
//     }
// };
