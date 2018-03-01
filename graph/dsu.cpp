#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct dsu
{
	int sz;
	vector<int> par, psz;

	void init(int n)
	{
		sz = n;
		par.assign(sz + 1, 0);
		psz.assign(sz + 1, 0);
		for(int i = 0; i <= sz; i++)
			par[i] = i, psz[i] = 1;
	}

	int root(int u) { return par[u] = ((u == par[u]) ? u : root(par[u])); }
	
	bool connected(int x, int y) { return root(x) == root(y); }

	void unite(int x, int y)
	{
		x = root(x), y = root(y);
		if(x == y) return;
		if(psz[x] > psz[y]) swap(x, y);
		par[x] = y, psz[y] += psz[x]; 
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

