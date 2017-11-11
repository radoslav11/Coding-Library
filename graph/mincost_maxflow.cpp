#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 10);
const int inf = (int)1e9 + 42;

int read_int();

struct edge
{
	int to, rev, flow, cap, cost;
	edge() { to = 0; rev = 0; flow = 0; cap = 0; cost = 0; }
	edge(int _to, int _rev, int _flow, int _cap, int _cost)
	{
		to = _to; rev = _rev;
		flow = _flow; cap = _cap;
		cost = _cost;
	}
};

int cnt_nodes = 0, s = MAXN - 1, t = MAXN - 2;
vector<edge> G[MAXN];

void add_edge(int u, int v, int w, int cost)
{
	edge t = edge(v, G[v].size(), 0, w, cost);
	edge r = edge(u, G[u].size(), 0, 0, -cost);

	G[u].push_back(t);
	G[v].push_back(r);
}

void build_graph()
{

}

deque<int> Q;
bool is_inside[MAXN];
int dist[MAXN], par_idx[MAXN], par[MAXN];

bool spfa()
{
	for(int i = 0; i <= cnt_nodes; i++)
		dist[i] = inf;
	dist[t] = inf;

	Q.clear();
	dist[s] = 0;
	is_inside[s] = true;
	Q.push_back(s);

	while(!Q.empty())
	{
		int u = Q.front();
		is_inside[u] = false;
		Q.pop_front();

		for(int i = 0; i < (int)G[u].size(); i++)
			if(G[u][i].cap > G[u][i].flow && dist[u] + G[u][i].cost < dist[G[u][i].to])
			{
				dist[G[u][i].to] = dist[u] + G[u][i].cost;
				par_idx[G[u][i].to] = i;
				par[G[u][i].to] = u;

				if(is_inside[G[u][i].to]) continue;
				if(!Q.empty() && dist[G[u][i].to] > dist[Q.front()]) Q.push_back(G[u][i].to);
				else Q.push_front(G[u][i].to);

				is_inside[G[u][i].to] = true;
			}
	}

	return dist[t] != inf;
}

pair<int, int> min_cost_flow(int flow)
{
	int f = 0, ret = 0;
	while(f <= flow && spfa())
	{
		int mn_flow = flow - f, u = t;
	    while(u != s)
		{
			mn_flow = min(mn_flow, G[par[u]][par_idx[u]].cap - G[par[u]][par_idx[u]].flow);
			u = par[u];
		}

		u = t;
		while(u != s)
		{
			G[par[u]][par_idx[u]].flow += mn_flow;
			G[u][G[par[u]][par_idx[u]].rev].flow -= mn_flow;
			ret += G[par[u]][par_idx[u]].cost * mn_flow;
			u = par[u];
		}

		f += mn_flow;
	}

	return make_pair(f, ret);
}

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

const int maxl = 100000;
char buff[maxl];
int ret_int, pos_buff = 0;

void next_char() { if(++pos_buff == maxl) fread(buff, 1, maxl, stdin), pos_buff = 0; }

int read_int()
{
	ret_int = 0;
	for(; buff[pos_buff] < '0' || buff[pos_buff] > '9'; next_char());
	for(; buff[pos_buff] >= '0' && buff[pos_buff] <= '9'; next_char())
		ret_int = ret_int * 10 + buff[pos_buff] - '0';
	return ret_int;
}
