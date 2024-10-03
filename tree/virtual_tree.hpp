#include <bits/stdc++.h>
#include <coding_library/tree/lca.hpp>
using namespace std;

class VirtualTree : public LCAUtils {
  public:
    VirtualTree() : LCAUtils() {}
    VirtualTree(int n) : LCAUtils(n) {}
    VirtualTree(int n, const vector<vector<int>>& adj, int root = 0)
        : LCAUtils(n, adj, root) {
        prepare(root);
    }

    void add_edge(int u, int v) { LCAUtils::add_edge(u, v); }

    pair<vector<vector<int>>, vector<int>> build(const vector<int>& vertices) {
        if(vertices.empty()) {
            return {{}, {}};
        }

        vector<int> vec = vertices;
        sort(vec.begin(), vec.end(), [&](int u, int v) {
            return in_time[u] < in_time[v];
        });
        vec.erase(unique(vec.begin(), vec.end()), vec.end());

        for(int i = (int)vec.size() - 1; i > 0; i--) {
            vec.push_back(lca(vec[i - 1], vec[i]));
        }

        sort(vec.begin(), vec.end(), [&](int u, int v) {
            return in_time[u] < in_time[v];
        });
        vec.erase(unique(vec.begin(), vec.end()), vec.end());

        vector<vector<int>> virtual_adj(vec.size());

        stack<int> s;
        s.push(0);
        for(int i = 1; i < (int)vec.size(); i++) {
            while(!upper(vec[s.top()], vec[i])) {
                int u = s.top();
                s.pop();
                int v = s.top();
                virtual_adj[v].push_back(u);
            }
            s.push(i);
        }

        while(s.size() > 1) {
            int u = s.top();
            s.pop();
            int v = s.top();
            virtual_adj[v].push_back(u);
        }

        return {virtual_adj, vec};
    }
};
