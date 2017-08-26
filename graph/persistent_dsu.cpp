#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct persistent_dsu
{
	struct state
	{
		int u, v, rnku, rnkv;
		state() {u = -1; v = -1; rnkv = -1; rnku = -1;}
		state(int _u, int _rnku, int _v, int _rnkv)
		{
			u = _u;
			rnku = _rnku;
			v = _v;
			rnkv = _rnkv;
		}
	};

	stack<state> st;
	int par[MAXN], depth[MAXN];
	persistent_dsu() {memset(par, -1, sizeof(par)); memset(depth, 0, sizeof(depth));}

	int root(int x)
	{
		if(x == par[x]) return x;
		return root(par[x]);
	}

	void init(int n)
	{
		for(int i = 0; i <= n; i++)
		{
			par[i] = i;
			depth[i] = 1;
		}
	}

	bool connected(int x, int y)
	{
		return root(x) == root(y);
	}

	void unite(int x, int y)
	{
		int rx = root(x), ry = root(y);
		st.push(state(rx, depth[rx], ry, depth[ry]));
		
		if(depth[rx] < depth[ry])
			par[rx] = ry;
		else if(depth[ry] < depth[rx])
			par[ry] = rx;
		else
		{
			par[rx] = ry;
			depth[rx]++;
		}
	}

	void backtrack(int c)
	{
		while(!st.empty() && c)
		{
			par[st.top().u] = st.top().u;
			par[st.top().v] = st.top().v;
			depth[st.top().u] = st.top().rnku;
			depth[st.top().v] = st.top().rnkv;
			st.pop();
			c--;
		}
	}
};

void read()
{

}

persistent_dsu d;

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

