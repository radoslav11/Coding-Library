#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

int n, m;
vector<int> G[MAXN];

void read()
{
	cin >> n >> m;

	for(int i = 0; i < m; i++)
	{
		int u, v;
		cin >> u >> v;

		G[u].push_back(v);
		G[v].push_back(u);
	}
}

stack<int> st;
int low[MAXN], disc[MAXN];
int dfs_time, bcc[MAXN], comps = 0;

void dfs(int u, int par)
{
	low[u] = dfs_time;
	disc[u] = dfs_time;
	dfs_time++;

	st.push(u);

	int sz = G[u].size(), v;
	for(int i = 0; i < sz; i++)
	{
		v = G[u][i];
		if(disc[v] == -1)
		{
			dfs(v, u);
			low[u] = min(low[u], low[v]);
		}
		else if(v != par)
			low[u] = min(low[u], disc[v]);
	}

	if(low[u] == disc[u])
	{
		comps++;
		while(st.top() != u)
		{
			bcc[st.top()] = comps;
			st.pop();
		}

		bcc[u] = comps;
		st.pop();
	}
}

void tarjan()
{
	memset(disc, -1, sizeof(disc));
	memset(low, -1, sizeof(low));
	memset(bcc, -1, sizeof(low));
	dfs_time = 0;
	comps = 0;

	while(!st.empty())
		st.pop();

	for(int i = 1; i <= n; i++)
		if(disc[i] == -1)
			dfs(i, -1);
}

void solve()
{
	tarjan();

    for(int i = 1; i <= n; i++)
        cout << i << " " << bcc[i] << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

