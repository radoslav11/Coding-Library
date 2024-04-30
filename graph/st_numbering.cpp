#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

int n, m, s, t;
vector<int> adj[MAXN];

void read() {
	cin >> n >> m >> s >> t;
	for(int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

int low[MAXN], disc[MAXN], dfs_time = 0, par[MAXN], sign[MAXN];
vector<int> preorder;

bool tarjan_check(int u, int pr = -1) {
	low[u] = disc[u] = ++dfs_time;

	int child_cnt = 0;
	for(int v: adj[u])
		if(v != pr) {
			if(disc[v] == -1) {
				child_cnt++;
				if(!tarjan_check(v, u)) return false;
				chkmin(low[u], low[v]);
				if(pr != -1 && low[v] >= disc[u]) return false;
			}
			else chkmin(low[u], disc[v]);
		}

	if(pr == -1 && child_cnt > 1) return false;
	return true;
}

void tarjan(int u, int pr = -1) {
	low[u] = disc[u] = ++dfs_time;
	for(int v: adj[u])
		if(v != pr) {
			if(disc[v] == -1) {
				preorder.push_back(v);
				tarjan(v, u);
				chkmin(low[u], low[v]);
				par[v] = u;
			}
			else chkmin(low[u], disc[v]);
		}
}

list<int> st_li;
list<int>::iterator it_ver[MAXN];

vector<int> st_numbering() {
	/// additional edge
	adj[s].push_back(t);
	adj[t].push_back(s);

	dfs_time = 0;
	preorder.clear();
	for(int i = 1; i <= n; i++) disc[i] = low[i] = -1, sign[i] = 0;

	if(!tarjan_check(t))
		return vector<int>(); /// no bipolar orientation

	for(int i = 1; i <= n; i++)
		if(disc[i] == -1)
			return vector<int>(); /// no bipolar orientation

	for(int i = 1; i <= n; i++) disc[i] = low[i] = -1, sign[i] = 0;

	dfs_time = 0;
	preorder.clear();
	disc[s] = low[s] = ++dfs_time;
	sign[disc[s]] = -1;
	tarjan(t);

	st_li.clear();
	st_li.push_back(s);
	st_li.push_back(t);

	it_ver[disc[s]] = st_li.begin();
	it_ver[disc[t]] = next(st_li.begin());

	for(int v: preorder) {
		if(sign[low[v]] == -1) it_ver[disc[v]] = st_li.insert(it_ver[disc[par[v]]], v);
		else it_ver[disc[v]] = st_li.insert(next(it_ver[disc[par[v]]]), v);
		sign[disc[par[v]]] = -sign[low[v]];
	}

	vector<int> ret(st_li.begin(), st_li.end());
	return ret;
}

void solve() {
	vector<int> li = st_numbering();
	for(int v: li) cout << v << " ";
	cout << endl;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
