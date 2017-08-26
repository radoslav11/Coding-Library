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

int d[MAXN], st[MAXN], en[MAXN], dfs_time = 0;
int par[MAXN][MAXLOG], vertex[MAXN];

void dfs_lca(int u, int de = 0)
{
    d[u] = de;
	st[u] = ++dfs_time;
    vertex[dfs_time] = u;
	for(int i = 1; i < MAXLOG; i++)
		par[u][i] = par[par[u][i - 1]][i - 1];

	for(int v: G[u])
		if(v != par[u][0])
		{
			par[v][0] = u;
			dfs_lca(v, de + 1);
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

void lca_precompute(int root)
{
	for(int i = 0; i < MAXLOG; i++)
        par[root][i] = root;

	dfs_time = 0;
	dfs_lca(root);
}

vector<int> vg[MAXN];
int mstack[MAXN], msize;

bool cmp(int u, int v) { return st[u] < st[v]; }

void build_tree(vector<int> vec)
{
    if(vec.empty()) { vg[0].clear(); return; }

    sort(vec.begin(), vec.end(), cmp);
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    int st_sz = vec.size();
    for(int i = 0; i + 1 < st_sz; i++)
        vec.push_back(lca(vec[i], vec[i + 1]));

    sort(vec.begin(), vec.end(), cmp);
    vec.erase(unique(vec.begin(), vec.end()), vec.end());

    for(int vertex: vec)
        vg[vertex].clear();

    msize = 0;
    mstack[msize++] = vec[0];

    for(int i = 1; i < vec.size(); i++)
    {
        int anc = lca(mstack[msize - 1], vec[i]);
        if(anc != mstack[msize - 1])
        {
            while(msize > 1 && d[mstack[msize - 2]] >= d[anc])
            {
                vg[mstack[msize - 2]].push_back(mstack[msize - 1]);
                msize--;
            }

            if(anc != mstack[msize - 1])
            {
                vg[anc].push_back(mstack[msize - 1]);
                msize--; mstack[msize++] = anc;
            }
        }

        mstack[msize++] = vec[i];
    }

    for(int i = 0; i < msize - 1; i++)
        vg[mstack[i]].push_back(mstack[i + 1]);
    vg[0].clear(); vg[0].push_back(mstack[0]);
}

void solve()
{
    d[0] = -1;
	lca_precompute(1);

	int q;
	cin >> q;
    for(int i = 0; i < q; i++)
    {
        int x, xx;
        cin >> x;
        vector<int> vec;

        for(int i = 0; i < x; i++)
        {
            cin >> xx;
            vec.push_back(xx);
        }

        /// has pseudo root 0
        build_tree(vec);
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

