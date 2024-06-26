#include <bits/stdc++.h>

using namespace std;
const int MAXLOG = 20;

int n;
vector<vector<int>> adj;

void read() {
    cin >> n;
    adj.assign(n + 1, {});
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

vector<int> in_time, out_time;
vector<vector<int>> par_up;

void dfs_lca(int u, int pr, int &dfs_time) {
    in_time[u] = ++dfs_time;

    par_up[u].resize(MAXLOG);
    par_up[u][0] = pr;
    for(int i = 1; i < MAXLOG; i++) {
        par_up[u][i] = par_up[par_up[u][i - 1]][i - 1];
    }

    for(int v: adj[u]) {
        if(v != pr) {
            dfs_lca(v, u, dfs_time);
        }
    }

    out_time[u] = dfs_time;
}

inline bool upper(int u, int v) {
    return in_time[u] <= in_time[v] && out_time[v] <= out_time[u];
}

int lca(int u, int v) {
    if(upper(u, v)) {
        return u;
    }
    if(upper(v, u)) {
        return v;
    }

    int a = u;
    for(int i = MAXLOG - 1; i >= 0; i--) {
        if(!upper(par_up[a][i], v)) {
            a = par_up[a][i];
        }
    }

    return par_up[a][0];
}

void lca_precompute(int root) {
    int dfs_time = 0;
    in_time.resize(n + 1);
    out_time.resize(n + 1);
    par_up.resize(n + 1);
    dfs_lca(root, root, dfs_time);
}

vector<vector<int>> vg;

int build_tree(vector<int> vec) {
    if(vec.empty()) {
        return -1;
    }

    function<bool(int, int)> cmp = [&](int u, int v) {
        return in_time[u] < in_time[v];
    };
    function<void(stack<int> &)> propagate_stack = [&](stack<int> &mstack) {
        int prev_top = mstack.top();
        mstack.pop();
        vg[mstack.top()].push_back(prev_top);
    };

    sort(vec.begin(), vec.end(), cmp);
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    for(int i = (int)vec.size() - 1; i > 0; i--) {
        vec.push_back(lca(vec[i - 1], vec[i]));
    }

    sort(vec.begin(), vec.end(), cmp);
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    for(int vertex: vec) {
        vg[vertex].clear();
    }

    stack<int> mstack;
    mstack.push(vec[0]);

    for(int i = 1; i < (int)vec.size(); i++) {
        while(!upper(mstack.top(), vec[i])) {
            propagate_stack(mstack);
        }

        mstack.push(vec[i]);
    }

    while(mstack.size() > 1) {
        propagate_stack(mstack);
    }

    return mstack.top();
}

void solve() {
    lca_precompute(1);
    vg.assign(n + 1, {});

    int q;
    cin >> q;
    for(int i = 0; i < q; i++) {
        int x, xx;
        cin >> x;
        vector<int> vec;

        for(int i = 0; i < x; i++) {
            cin >> xx;
            vec.push_back(xx);
        }

        int root = build_tree(vec);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
