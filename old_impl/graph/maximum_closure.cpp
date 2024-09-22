#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

template<class FlowT>
struct max_flow
{
    const static FlowT finf = 1e18 + 42 + 17;
    const static FlowT feps = 0;

    struct edge
    {
        FlowT flow, cap;
        int idx, rev, to;
        edge() { flow = 0; cap = 0; rev = 0; idx = 0; to = 0; }
        edge(int _to, int _rev, FlowT _flow, FlowT _cap, int _idx)
        {
            to = _to; rev = _rev;
            flow = _flow; cap = _cap;
            idx = _idx;
        }
    };

    vector<edge> G[MAXN];
    int n, dist[MAXN], po[MAXN];

    bool bfs(int s, int t)
    {
        dist[s] = -1, po[s] = 0;
        dist[t] = -1, po[t] = 0;
        for(int v = 0; v <= n; v++)
            dist[v] = -1, po[v] = 0;

        queue<int> Q;
        Q.push(s);
        dist[s] = 0;

        while(!Q.empty())
        {
            int u = Q.front();
            Q.pop();

            for(edge e: G[u])
                if(dist[e.to] == -1 && e.flow < e.cap)
                {
                    dist[e.to] = dist[u] + 1;
                    Q.push(e.to);
                }
        }

        return dist[t] != -1;
    }

    FlowT dfs(int u, int t, FlowT fl = finf)
    {
        if(u == t)
            return fl;

        for(; po[u] < G[u].size(); po[u]++)
        {
            auto &e = G[u][po[u]];
            if(dist[e.to] == dist[u] + 1 && e.flow < e.cap)
            {
                FlowT f = dfs(e.to, t, min(fl, e.cap - e.flow));

                e.flow += f;
                G[e.to][e.rev].flow -= f;

                if(f > 0)
                    return f;
            }
        }

        return 0;
    }

    void init(int _n) { n = _n; for(int i = 0; i <= n; i++) G[i].clear(); }

    void add_edge(int u, int v, FlowT w, int idx = -1)
    {
        G[u].push_back(edge(v, G[v].size(), 0, w, idx));
        G[v].push_back(edge(u, G[u].size() - 1, 0, 0, -1));
    }

    FlowT flow(int s, int t)
    {
        if(s == t) return finf;

        FlowT ret = 0, to_add;
        while(bfs(s, t))
            while((to_add = dfs(s, t)))
                ret += to_add;

        return ret;
    }
};

template<class T>
struct maximum_closure
{
	int n;
	T w[MAXN];
	max_flow<T> mf;
	vector<int> adj[MAXN];

	void init(int _n)
	{
		n = _n;
		for(int i = 1; i <= n; i++)
			w[i] = 0, adj[i].clear();
	}

	void add_clause(int i, int j) { adj[i].push_back(j); }

	int dfs_time, cnt_comp, comp[MAXN], disc[MAXN], low[MAXN];
	bool in_stack[MAXN];
	stack<int> st;

	void dfs_tarjan(int u)
	{
		disc[u] = low[u] = ++dfs_time;
		in_stack[u] = 1;
		st.push(u);

		for(int v: adj[u])
			if(disc[v] == -1)
			{
				dfs_tarjan(v);
				chkmin(low[u], low[v]);
			}
			else if(in_stack[v])
				chkmin(low[u], disc[v]);
	
		if(low[u] == disc[u])
		{
			cnt_comp++;
			while(st.top() != u)
			{
				in_stack[st.top()] = 0;
				comp[st.top()] = cnt_comp;
				st.pop();
			}

			comp[u] = cnt_comp;
			in_stack[u] = 0;
			st.pop();
		}
	}

	T solve()
	{
		for(int i = 1; i <= n; i++)		
			disc[i] = -1;

		dfs_time = 0, cnt_comp = 0;
		for(int i = 1; i <= n; i++)
			if(disc[i] == -1)
				dfs_tarjan(i);

		int s = cnt_comp + 1, t = cnt_comp + 2;
		mf.init(cnt_comp + 3);

		vector<T> new_w;
		new_w.assign(cnt_comp + 1, 0);
		for(int i = 1; i <= n; i++)
			new_w[comp[i]] += w[i];

		for(int i = 1; i <= n; i++)
			for(int j: adj[i])
				mf.add_edge(comp[i], comp[j], max_flow<T>::finf);

		T sum = 0;
		for(int i = 1; i <= cnt_comp; i++)
		{
			if(new_w[i] < 0) mf.add_edge(s, i, -new_w[i]);
			else mf.add_edge(i, t, new_w[i]), sum += new_w[i]; 
		}

		return sum - mf.flow(s, t);
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

