#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (52);

int G[MAXN][MAXN], n;
int mn_deg, comp_size;
bitset<MAXN> st;
vector<int> adj[MAXN];
bool visited[MAXN];

int get_deg(int u) {
    int res = 0;
    for(int v = 0; v < n; v++)
        if(st[v]) res += G[u][v];

    return res;
}

void dfs(int u) {
    visited[u] = true;
    mn_deg = min(mn_deg, (int)adj[u].size());
    comp_size++;
    for(int v: adj[u])
        if(!visited[v]) dfs(v);
}

int brute() {
    for(int u = 0; u < n; u++)
        if(st[u]) visited[u] = false, adj[u].clear();
    for(int u = 0; u < n; u++)
        if(st[u])
            for(int v = 0; v < n; v++)
                if(G[u][v] && st[v]) adj[u].push_back(v);

    int res = 0;
    for(int u = 0; u < n; u++)
        if(st[u] && !visited[u]) {
            mn_deg = MAXN;
            comp_size = 0;
            dfs(u);

            if(mn_deg <= 1)
                res += ((comp_size + 1) / 2);
            else
                res += (comp_size / 2);
        }

    return res;
}

int rec() {
    if(!st.count()) return 0;

    int d = -1;
    for(int v = 0; v < n; v++)
        if(st[v] && (d == -1 || get_deg(v) > get_deg(d))) d = v;

    if(get_deg(d) <= 2) return brute();

    int ret = 0;
    bitset<MAXN> prv = st;

    st[d] = 0;
    ret = max(ret, rec());

    st = prv;
    st[d] = 0;
    for(int u = 0; u < n; u++)
        if(G[u][d]) st[u] = 0;

    ret = max(ret, 1 + rec());

    st = prv;
    return ret;
}

/// O(1.38 ^ n) worst case
int max_anticlique() {
    for(int i = 0; i < n; i++) st[i] = 1;
    return rec();
}

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
