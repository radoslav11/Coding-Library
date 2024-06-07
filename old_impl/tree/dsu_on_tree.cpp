#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

int n;
vector<int> G[MAXN];

void read() {
    cin >> n;

    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        G[u].push_back(v);
        G[v].push_back(u);
    }
}

int tr_sz[MAXN];

void pre_dfs(int u, int pr) {
    tr_sz[u] = 1;
    for(int v: G[u]) {
        if(v != pr) {
            pre_dfs(v, u);
            tr_sz[u] += tr_sz[v];
        }
    }
}

void add_vertex(int u) {}

void clear(int u, int pr) {}

void add(int u, int pr) {
    add_vertex(u);
    for(int v: G[u]) {
        if(v != pr) {
            add(v, u);
        }
    }
}

void dfs(int u, int pr, int keep) {
    pair<int, int> mx = {-1, -1};
    for(int v: G[u]) {
        if(v != pr) {
            mx = max(mx, {tr_sz[v], v});
        }
    }

    for(int v: G[u]) {
        if(v != pr && v != mx.second) {
            dfs(v, u, 0);
        }
    }

    if(mx.second != -1) {
        dfs(mx.second, u, 1);
    }

    for(int v: G[u]) {
        if(v != pr && v != mx.second) {
            add(v, u);
        }
    }

    add_vertex(u);

    if(keep) {
        return;
    }
    clear(u, pr);
}

void solve() {
    pre_dfs(1, -1);
    dfs(1, -1, 1);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
