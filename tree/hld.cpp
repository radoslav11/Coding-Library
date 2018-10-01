#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
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

int tr_sz[MAXN];

int pre_dfs(int u, int pr)
{
	tr_sz[u] = 1;
	for(int v: adj[u])
		if(v != pr)
			tr_sz[u] += pre_dfs(v, u);

	return tr_sz[u];
}

int head[MAXN], par[MAXN][20], st[MAXN], en[MAXN], dfs_time;

void decompose(int u, int pr, int head)
{
	::head[u] = head;
	st[u] = ++dfs_time;

	par[u][0] = pr;
	for(int i = 1; i < 20; i++)
		par[u][i] = par[par[u][i - 1]][i - 1];

	int mx_sz = -1, f_c = -1;
	for(int v: adj[u])
		if(v != pr)
			if(chkmax(mx_sz, tr_sz[v]))
				f_c = v;

	if(f_c != -1)
		decompose(f_c, u, head);

	for(int v: adj[u])
		if(v != pr && v != f_c)
			decompose(v, u, v);

	en[u] = dfs_time;
}

inline int upper(int u, int v) { return st[u] <= st[v] && en[v] <= en[u];  }

int lca(int u, int v)
{
	if(upper(u, v)) return u;
	if(upper(v, u)) return v;

	for(int i = 19; i >= 0; i--)
		if(!upper(par[u][i], v))
			u = par[u][i];

	return par[u][0];
}

void hld_precompute(int root)
{
	pre_dfs(root, root);
	decompose(1, 1, 1);
}

vector<pair<int, int> > get_path_up(int u, int anc)
{
	vector<pair<int, int> > ret;
	while(st[anc] < st[u])
	{
		ret.push_back({max(st[anc] + 1, st[head[u]]), st[u]});
		u = par[head[u]][0];
	}

	return ret;
}

vector<pair<int, int> > get_path(int u, int v)
{
	int l = lca(u, v);
	vector<pair<int, int> > ret = get_path_up(u, l);
	
	// if we consider vertices, not edges
	// ret.push_back({st[l], st[l]});

	vector<pair<int, int> > oth = get_path_up(v, l);
	reverse(oth.begin(), oth.end());
	
	// if the path is directed
	// for(auto &it: oth) swap(it.first, it.second);
	
	for(auto it: oth) ret.push_back(it);

	return ret;
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

