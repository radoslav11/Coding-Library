#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int MAXLOG = 20;

int n, w[MAXN];
vector<int> adj[MAXN];

void read()
{
	cin >> n;
	for(int i = 1; i <= n; i++)
		cin >> w[i];

	for(int i = 1; i <= n - 1; i++)
	{
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

int st[MAXN], en[MAXN], dfs_time, par[MAXN][MAXLOG], par_mn[MAXN][MAXLOG];

void pre_dfs(int u, int pr)
{
	par[u][0] = pr;
	par_mn[u][0] = w[u];
	st[u] = ++dfs_time;

	for(int i = 1; i < MAXLOG; i++)
	{
		par[u][i] = par[par[u][i - 1]][i - 1];
		par_mn[u][i] = min(par_mn[u][i - 1], par_mn[par_mn[u][i - 1]][i - 1]);
	}

	for(int v: adj[u])
		if(v != pr)
			pre_dfs(v, u);

	en[u] = dfs_time;
}

inline bool upper(int u, int v) { return st[u] <= st[v] && en[v] <= en[u];  }

int lca(int u, int v)
{
	if(upper(u, v)) return u;
	if(upper(v, u)) return v;

	for(int l = MAXLOG - 1; l >= 0; l--)
		if(!upper(par[u][l], v))
			u = par[u][l];

	return par[u][0];
}

int get_path_up(int u, int anc)
{
	if(upper(u, anc)) return (int)1e9;

	int ans = INT_MAX;
	for(int l = MAXLOG - 1; l >= 0; l--)
		if(!upper(par[u][l], anc))
		{
			ans = min(ans, par_mn[u][l]);
			u = par[u][l];
		}
	
	return ans;
}

int get_path(int u, int v)
{
	int anc = lca(u, v), ans = w[anc];
	ans = min(get_path_up(u, anc), get_path_up(v, anc));
	return ans;
}

void lca_precompute(int root)
{
	dfs_time = 0;
	pre_dfs(root, root);
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

