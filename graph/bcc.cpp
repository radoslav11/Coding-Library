#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template <class T, class T2> inline int chkmax(T &x, const T2 &y) {
	return x < y ? x = y, 1 : 0;
}
template <class T, class T2> inline int chkmin(T &x, const T2 &y) {
	return x > y ? x = y, 1 : 0;
}
const int MAXN = (1 << 20);

template <class T> int test() { return 0; }

int n, m;
vector<int> adj[MAXN];

void read() {
	cin >> n >> m;
	for(int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

int low[MAXN], disc[MAXN], dfs_time;
vector<vector<pair<int, int> > > bcc;
vector<pair<int, int> > st, bridges;

void tarjan(int u, int pr = -1) {
	low[u] = disc[u] = ++dfs_time;

	int child_cnt = 0;
	for(int v: adj[u])
		if(v != pr) {
			if(disc[v] == -1) {
				st.push_back({u, v});
				tarjan(v, u);
				child_cnt++;
				chkmin(low[u], low[v]);

				if((pr == -1 && child_cnt > 1) ||
				   (pr != -1 && disc[u] <= low[v])) {
					vector<pair<int, int> > curr = {make_pair(u, v)};
					while(st.back() != make_pair(u, v)) {
						curr.push_back(st.back());
						st.pop_back();
					}

					st.pop_back();
					if(curr.size() == 1)
						bridges.push_back(curr.back());
					else
						bcc.push_back(curr);
				}
			} else if(disc[v] < disc[u]) {
				chkmin(low[u], disc[v]);
				st.push_back({u, v});
			}
		}
}

void compute_bcc() {
	dfs_time = 0;
	for(int i = 1; i <= n; i++) low[i] = disc[i] = -1;
	for(int i = 1; i <= n; i++)
		if(disc[i] == -1) {
			st.clear();
			tarjan(i);
			if(!st.empty()) {
				if(st.size() == 1)
					bridges.push_back(st.back());
				else
					bcc.push_back(st);
				st.clear();
			}
		}
}

void solve() {}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
