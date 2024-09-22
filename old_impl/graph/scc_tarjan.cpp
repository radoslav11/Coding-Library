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
	}
}

stack<int> st;
vector<vector<int>> scc; 
int low[MAXN], disc[MAXN], comp[MAXN];
int dfs_time;
bool in_stack[MAXN];

void dfs(int u)
{
	low[u] = dfs_time;
	disc[u] = dfs_time;
	dfs_time++;

	in_stack[u] = true;
	st.push(u);
	
	int sz = G[u].size(), v;
	for(int i = 0; i < sz; i++)
	{
		v = G[u][i];
	
		if(disc[v] == -1)
		{
			dfs(v);
			low[u] = min(low[u], low[v]);
		}
		else if(in_stack[v] == true)
			low[u] = min(low[u], disc[v]);
	}

	if(low[u] == disc[u])
	{
		scc.push_back(vector<int>());
		while(st.top() != u)
		{
			scc[scc.size() - 1].push_back(st.top());
			in_stack[st.top()] = false;
			st.pop();
		}

		scc[scc.size() - 1].push_back(u);
		in_stack[u] = false;
		st.pop();
	}
}

void tarjan()
{
	memset(comp, -1, sizeof(comp));
	memset(disc, -1, sizeof(disc));
	memset(low, -1, sizeof(low));
	memset(in_stack, 0, sizeof(in_stack));
	dfs_time = 0;

	while(!st.empty())
		st.pop();

	for(int i = 1; i <= n; i++)
		if(disc[i] == -1)
			dfs(i);

	int sz = scc.size();
	for(int i = 0; i < sz; i++)
		for(int j = 0; j < (int)scc[i].size(); j++)
			comp[scc[i][j]] = i;
}

void solve()
{
	tarjan();
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

