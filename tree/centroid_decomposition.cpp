#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

int n;
vector<int> adj[MAXN];

void read()
{
	cin >> n;
	for(int i = 0; i < n - 1; i++)
	{
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

int tr_sz[MAXN], cnt_vers;
bool used[MAXN];

void pre_dfs(int u, int pr)
{
	cnt_vers++;
	tr_sz[u] = 1;
	for(int v: adj[u])
		if(!used[v] && v != pr)
		{
			pre_dfs(v, u);
			tr_sz[u] += tr_sz[v];
		}
}

int centroid(int u, int pr)
{
	for(int v: adj[u])
		if(!used[v] && v != pr && tr_sz[v] > cnt_vers / 2)
			return centroid(v, u);

	return u;
}

int link[MAXN]; 
 
void decompose(int u, int pr = -1)
{
	cnt_vers = 0;
	pre_dfs(u, u);
	int cen = centroid(u, u);
	link[cen] = pr;

	used[cen] = true;
	for(int v: adj[cen])
		if(!used[v]) 
			decompose(v, cen);
	used[cen] = false;
}

void solve()
{

}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

