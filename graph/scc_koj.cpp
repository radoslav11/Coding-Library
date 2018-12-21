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

struct SCC
{
	int n, comp[MAXN];
	bool visited[MAXN];
	vector<int> adj[MAXN], radj[MAXN], comp_ids, top_sort;

	void add_edge(int u, int v)
	{
		adj[u].pb(v);
		radj[v].pb(u);
	}

	void init(int n)
	{
		comp_ids.clear();
		top_sort.clear();
		for(int i = 0; i <= n; i++)
			adj[i].clear(), radj[i].clear();
	}

	void dfs1(int u)
	{
		visited[u] = true;
		for(int v: adj[u])
			if(!visited[v])
				dfs1(v);
		top_sort.pb(u);
	}

	void dfs2(int u)
	{
		for(int v: radj[u])
			if(comp[v] == -1)
			{
				comp[v] = comp[u];
				dfs2(v);
			}
	}

	void solve()
	{
		for(int i = 0; i <= n; i++) comp[i] = -1, visited[i] = 0;
		for(int i = 1; i <= n; i++) 
			if(!visited[i]) 
				dfs1(i);

		reverse(ALL(top_sort));
		for(int u: top_sort)
			if(comp[u] == -1)
			{
				comp[u] = u;
				comp_ids.pb(u);
				dfs2(u);
			}
	}
};

void read()
{

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

