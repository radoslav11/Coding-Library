#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int MAXLOG = 24;

int st[MAXN], en[MAXN];
int par[MAXN][MAXLOG], lgn, dfs_time;
int tr_sz[MAXN], head[MAXN], root, vert[MAXN];

struct node
{
	int sum;

	node() {sum = 0;}
	node(int val)
	{
		sum = val;
	}
};

node temp, broken;

node merge(node l, node r)
{
	temp.sum = l.sum + r.sum;
	return temp;
}

struct segment_tree
{
	node tr[4 * MAXN];

	void init(int l, int r, int idx)
	{
		if(l == r)
		{
			tr[idx] = node(vert[l]);
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void update(int pos, int val, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			tr[idx].sum += val;
			return;
		}

		int mid = (l + r) >> 1;
		update(pos, val, l, mid, 2 * idx + 1);
		update(pos, val, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	node query(int qL, int qR, int l, int r, int idx)
	{
		if(l > qR || r < qL)
			return broken;

		if(qL <= l && r <= qR)
			return tr[idx];

		int mid = (l + r) >> 1;
		return merge(query(qL, qR, l, mid, 2 * idx + 1), query(qL, qR, mid + 1, r, 2 * idx + 2));
	}
};

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

segment_tree t;

void pre_dfs(int u, int pr)
{
	tr_sz[u] = 1;
	for(int v: G[u])
		if(v != pr)
		{
			pre_dfs(v, u);
			tr_sz[u] += tr_sz[v];
		}
}

void decompose(int u, int head, int pr)
{
	par[u][0] = pr;
	for(int l = 1; l <= lgn; l++)
		par[u][l] = par[par[u][l - 1]][l - 1];

	st[u] = ++dfs_time;
	vert[dfs_time] = u;
	::head[u] = head;

	pair<int, int> mx = {-1, -1};
	for(int v: G[u])
		if(v != pr)
			mx = max(mx, {tr_sz[v], v});

	if(mx.second != -1)
		decompose(mx.second, head, u);

	for(int v: G[u])
		if(v != pr && v != mx.second)
			decompose(v, v, u);

	en[u] = dfs_time;
}

inline bool upper(int u, int v) { return st[u] <= st[v] && en[v] <= en[u]; }

int lca(int u, int v)
{
	if(upper(u, v)) return u;
	if(upper(v, u)) return v;

	for(int l = lgn; l >= 0; l--)
		if(!upper(par[u][l], v))
			u = par[u][l];

	return par[u][0];
}

void hld_precompute(int _root)
{
	root = _root;
	lgn = 0; while((1 << lgn) < n) lgn++;
	dfs_time = 0;

	pre_dfs(root, root);
	decompose(root, root, root);
}

node get_path_up(int u, int anc)
{
	node ret = broken;
	while(st[anc] < st[u])
	{
		ret = merge(ret, t.query(max(st[anc] + 1, st[head[u]]), st[u], 1, dfs_time, 0));
		u = par[head[u]][0];
	}

	return ret;
}

void solve()
{
	hld_precompute(1);
	t.init(1, dfs_time, 0);


}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

