#include <bits/stdc++.h>

using namespace std;

template<class T, class T1>
int chkmin(T &x, const T1 &y) {
    return x > y ? x = y, 1 : 0;
}

template<class T, class T1>
int chkmax(T &x, const T1 &y) {
    return x < y ? x = y, 1 : 0;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, vector<T> &a) {
    for(auto &x: a) {
        out << x << ' ';
    }
    return out;
};

/*
    https://codesprintla24.kattis.com/contests/codesprintla24open/problems/catbusplan

    Problem Statement:
    Split a graph into k edge-disjoint paths.

    Note that the Eulerian path algorithm works only when there are at most 2 odd 
    degree vertices, as otherwise, when starting the DFS from a degree of odd degree,
    you might disconnect the graph (and the splicing of the cycles on the path doesn't 
    work). To fix this, we add fake edges between all but one pair of vertices of odd 
    degree, and then start the DFS from one of the 2 odd vertices that are left.
*/

int n, m, k;
vector<vector<pair<int, int>>> adj;
vector<pair<int, int>> edges;

void read() {
    cin >> n >> m >> k;
    adj.assign(n, {});
    edges.resize(m);
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back({v, i * 2});
        adj[v].push_back({u, i * 2 + 1});
        edges[i] = {u, v};
    }
}

vector<int> answer;
vector<bool> used;
vector<int> po;
vector<int> deg;

void dfs(int u) {
    for(; po[u] < (int)adj[u].size();) {
        int idx = po[u]++;
        if(!used[adj[u][idx].second >> 1]) {
            used[adj[u][idx].second >> 1] = true;
            dfs(adj[u][idx].first);
            answer.push_back(adj[u][idx].second);
        }
    }
}

pair<int, int> get_edge(int edge_i) {
    if(edge_i & 1) {
        return edges[edge_i >> 1];
    } else {
        return {edges[edge_i >> 1].second, edges[edge_i >> 1].first};
    }
}

void dfs_visit_nodes(int u, vector<bool> &visited, vector<int> &nodes) {
    nodes.push_back(u);
    visited[u] = true;
    for(auto [v, edge_i]: adj[u]) {
        if(!visited[v]) {
            dfs_visit_nodes(v, visited, nodes);
        }
    }
}

void solve() {
    if(k > m) {
        cout << "Impossible\n";
        return;
    }

    used.assign(m, false);
    po.assign(n, 0);
    deg.assign(n, 0);

    vector<bool> used_vers(n, false);

    for(int i = 0; i < n; i++) {
        deg[i] = adj[i].size();
    }

    int broken = m;

    vector<vector<int>> pre_paths;
    for(int u = 0; u < n; u++) {
        if(!used_vers[u]) {
            vector<int> nodes_here;
            dfs_visit_nodes(u, used_vers, nodes_here);

            vector<pair<int, int>> broken_edges_here;
            int last_odd = -1;
            for(int i: nodes_here) {
                if(deg[i] % 2 == 1) {
                    if(last_odd != -1) {
                        broken_edges_here.push_back({last_odd, i});
                        last_odd = -1;
                    } else {
                        last_odd = i;
                    }
                }
            }

            while((int)broken_edges_here.size() > 1) {
                auto [i, j] = broken_edges_here.back();
                adj[i].push_back({j, 2 * broken});
                adj[j].push_back({i, 2 * broken + 1});
                broken++;
                used.push_back(false);
                edges.push_back({i, j});
                deg[i]++;
                deg[j]++;
                broken_edges_here.pop_back();
            }

            answer.clear();
            if(!broken_edges_here.empty()) {
                dfs(broken_edges_here[0].first);
            } else {
                dfs(u);
            }

            pre_paths.push_back(answer);
        }
    }

    vector<vector<int>> paths;
    for(auto path: pre_paths) {
        vector<int> new_path;
        for(int x: path) {
            if(x >= 2 * m) {
                if(!new_path.empty()) {
                    paths.push_back(new_path);
                }
                new_path.clear();
            } else {
                new_path.push_back(x);
            }
        }

        if(!new_path.empty()) {
            paths.push_back(new_path);
        }
    }

    if((int)paths.size() > k) {
        cout << "Impossible\n";
        return;
    }

    int need = k - (int)paths.size();
    for(int i = 0; i < (int)paths.size(); i++) {
        while((int)paths[i].size() > 1 && need > 0) {
            paths.push_back({paths[i].back()});
            paths[i].pop_back();
            need--;
        }
    }

    assert(need == 0);
    cout << "Possible\n";
    for(auto &path: paths) {
        cout << get_edge(path[0]).first + 1 << ' ';
        for(auto edge_i: path) {
            cout << get_edge(edge_i).second + 1 << ' ';
        }

        cout << endl;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
