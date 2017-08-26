#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int MAXLOG = 20;

int n;
vector<int> G[MAXN];

void read()
{
	cin >> n;
	for(int i = 0; i < n - 1; i++)
	{
		int u, v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}
}

int st[MAXN], en[MAXN], dfs_time = 0;
int par[MAXN][MAXLOG];

void dfs_lca(int u)
{
	st[u] = ++dfs_time;
	for(int i = 1; i < MAXLOG; i++)
		par[u][i] = par[par[u][i - 1]][i - 1];

	for(int v: G[u])
		if(v != par[u][0])
		{
			par[v][0] = u;
			dfs_lca(v);
		}

	en[u] = dfs_time;
}

inline bool upper(int u, int v) { return st[u] <= st[v] && en[v] <= en[u]; }

int lca(int u, int v)
{
	if(upper(u, v)) return u;
	if(upper(v, u)) return v;

	int a = u;
	for(int i = MAXLOG - 1; i >= 0; i--)
		if(!upper(par[a][i], v))
			a = par[a][i];

	return par[a][0];
}

int parent(int u, int up)
{
	int a = u;
	for(int i = MAXLOG - 1; i >= 0; i--)
		if(up & (1 << i))
			a = par[a][i];

	return a;
}

void lca_precompute(int root)
{
	for(int i = 0; i < MAXLOG; i++)
        par[root][i] = root;

	dfs_time = 0;
	dfs_lca(root);
}

void solve()
{
	lca_precompute(1);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

