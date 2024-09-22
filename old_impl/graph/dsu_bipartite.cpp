#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

int n, m;
pair<int, int> e[MAXN];

void read() {
	cin >> n >> m;
	for(int i = 0; i < m; i++) cin >> e[i].first >> e[i].second;
}

int ds_sz[MAXN], col[MAXN], par[MAXN];
vector<int> li[MAXN];

int root(int u) {
	if(u == par[u]) return u;
	return (par[u] = root(par[u]));
}

void unite(int u, int v) {
	if(root(u) == root(v)) {
		if(col[u] == col[v]) {
			cout << "NO" << endl;
			exit(0);
		}

		return;
	}

	if(li[root(u)].size() > li[root(v)].size()) swap(u, v);

	if(col[u] == col[v])
		for(int ver: li[root(u)]) col[ver] ^= 1;

	for(int ver: li[root(u)]) li[root(v)].push_back(ver);

	par[root(u)] = root(v);
}

void solve() {
	for(int i = 1; i <= n; i++)
		ds_sz[i] = 1, col[i] = 1, li[i].push_back(i), par[i] = i;

	for(int i = 0; i < m; i++) unite(e[i].first, e[i].second);

	cout << "YES" << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
