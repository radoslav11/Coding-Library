#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int MAXLOG = 20;

struct sparse_table_pair
{
	pair<int, int> dp[MAXN << 1][MAXLOG];
	int prec_lg2[MAXN << 1], n;

	sparse_table_pair() { }

	void init(vector<pair<int, int> > &a)
	{
		n = a.size();
		for(int i = 2; i < 2 * n; i++) prec_lg2[i] = prec_lg2[i >> 1] + 1;
		for(int i = 0; i < n; i++) dp[i][0] = a[i];
		for(int j = 1; (1 << j) <= n; j++)    
			for(int i = 0; i < n; i++)
				dp[i][j] = min(dp[i][j - 1], dp[i + (1 << (j - 1))][j - 1]);
	}

	pair<int, int> query(int l, int r) 
	{
		int k = prec_lg2[r - l + 1];
		return min(dp[l][k], dp[r - (1 << k) + 1][k]);
	}
};

struct LCA
{
	int dep[MAXN];
	int pos[MAXN];

	sparse_table_pair rmq;
	vector<pair<int, int> > order;
	vector<int> adj[MAXN];
	
	void add_edge(int u, int v)
	{
		adj[u].pb(v);
		adj[v].pb(u);
	}

	void pre_dfs(int u, int pr = -1, int d = 0)
	{
		pos[u] = SZ(order);
		dep[u] = d;
		order.pb({d, u});

		for(int v: adj[u])
			if(v != pr)
			{
				pre_dfs(v, u, d + 1);
				order.pb({d, u});
			}
	}

	void clear(int n)
	{
		order.clear();
		for(int i = 0; i <= n; i++)
			adj[i].clear();
	}

	void init(int root)
	{
		order.clear();
		pre_dfs(root);
		rmq.init(order);
	}

	int lca(int u, int v)
	{
		if(pos[u] > pos[v]) swap(u, v);
		return rmq.query(pos[u], pos[v]).second;
	}

	int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
};

int n;
LCA T;

void read()
{
	cin >> n;
	for(int i = 0; i < n - 1; i++)
	{
		int u, v;
		cin >> u >> v;
		T.add_edge(u, v);
	}
}

void solve()
{
	T.init(1);

	int q;
	cin >> q;
	while(q--)
	{
		int u, v;
		cin >> u >> v;
		cout << T.lca(u, v) << " " << T.dist(u, v) << endl;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

