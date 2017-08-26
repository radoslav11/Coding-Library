#include <bits/stdc++.h>
#define endl '\n'

#define real real_word

using namespace std;
const int MAXN = (1 << 20);
const int inf = (int)1e9 + 42;

int n;
vector<pair<int, int> > G[MAXN];

void read()
{
	cin >> n;

	for(int i = 0; i < n - 1; i++)
	{
		int u, v, w;
		cin >> u >> v >> w;

		G[u].push_back(make_pair(v, w));
		G[v].push_back(make_pair(u, w));
	}
}

int psz = 0;
vector<int> path;
int real[MAXN], id[MAXN], d[MAXN];
int first_occ[MAXN];
bool visited[MAXN];

struct segment_tree_rmq
{
	int tr[4 * MAXN];

	void init(int l, int r, int idx)
	{
		if(l == r)
		{
			tr[idx] = path[l];
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = min(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	int query(int qL, int qR, int l, int r, int idx)
	{
		if(qL <= l && r <= qR)
			return tr[idx];

		if(qL > r || l > qR)
			return inf;

		int mid = (l + r) >> 1;
		return min(query(qL, qR, l, mid, 2 * idx + 1), query(qL, qR, mid + 1, r, 2 * idx + 2));
	}
};

segment_tree_rmq rmq;

int lca(int v, int u)
{
	int l = first_occ[id[u]], r = first_occ[id[v]];
	if(r < l) swap(l, r);
	return real[rmq.query(l, r, 0, psz - 1, 0)];
}

int dist(int v, int u) {return d[v] + d[u] - 2 * d[lca(v, u)];}

void euler_dfs(int u) 
{
	path.push_back(id[u]);
	visited[u] = true;

	int sz = G[u].size(), v;
	for(int i = 0; i < sz; i++) 
	{
		v = G[u][i].first;

		if(!visited[v])
			euler_dfs(v), path.push_back(id[u]);
	}
}

void rank_bfs(int src)
{	
	memset(d, -1, sizeof(d));

	int c = 0, u, v, sz;
	queue<int> Q;
	d[src] = 0;
	id[src] = ++c;
	real[c] = src;
	Q.push(src);

	while(!Q.empty())
	{
		u = Q.front();
		Q.pop();

		sz = G[u].size();
		for(int i = 0; i < sz; i++)
		{
			v = G[u][i].first;

			if(d[v] == -1)
			{
				d[v] = d[u] + G[u][i].second;
				id[v] = ++c;
				real[c] = v;
				Q.push(v);
			}
		}
	}
}	

void lca_precompute()
{
	rank_bfs(1);
	euler_dfs(1);
	psz = path.size();
	rmq.init(0, psz - 1, 0);

	memset(first_occ, -1, sizeof(first_occ));
	for(int i = 0; i < psz; i++)
		if(first_occ[path[i]] == -1) first_occ[path[i]] = i;
}

void solve()
{
	lca_precompute();
	
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

