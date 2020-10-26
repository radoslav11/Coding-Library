#include <bits/stdc++.h>
#define endl '\n'

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int inf = (int)1e9 + 42; 

int n;
vector<int> adj[MAXN];

void read() {
	cin >> n;
	for(int i = 0; i < n - 1; i++) {
		int u, v;
		cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
}

int st[MAXN], en[MAXN], ver[MAXN], dep[MAXN], dfs_time = 0;
vector<array<int, 1>> dat;
void pre_dfs(int u, int pr, int d = 0) {
	en[u] = st[u] = dfs_time++;
	ver[st[u]] = u;
	dep[u] = d;

	dat.push_back({dep[u]});

	for(int v: adj[u]) {
		if(v != pr) {
			pre_dfs(v, u, d + 1);
			dat.push_back({dep[u]});
			en[u] = dfs_time++;
			ver[en[u]] = u;
		}
	}
}

struct segment_tree {
	struct node {
		int l, m, r, lm, mr, lmr;
		node() { 
			l = m = r = lm = mr = lmr = -inf; 
		}

		node(int d) {	
			l = d;
			m = -2 * d;
			r = d;
			lm = -d;
			mr = -d;
			lmr = 0;
		}
	};
	
	node merge(node a, node b) {
		node ret;
		ret.l = max(a.l, b.l);
		ret.r = max(a.r, b.r);
		ret.lm = max(a.lm, b.lm);
		ret.mr = max(a.mr, b.mr);
		ret.lmr = max(a.lmr, b.lmr);

		chkmax(ret.lm, a.l + b.m);
		chkmax(ret.mr, a.m + b.r);

		chkmax(ret.lmr, a.lm + b.r);
		chkmax(ret.lmr, a.l + b.mr);
		return ret;
	} 	

	int lazy[4 * MAXN];
	node tr[4 * MAXN];	

	void push(int l, int r, int idx) {
		if(lazy[idx]) {
			if(l != r) {
				lazy[2 * idx + 1] ^= lazy[idx];
				lazy[2 * idx + 2] ^= lazy[idx];
			}
			lazy[idx] = 0;
		}
	}

	void init(int l, int r, int idx, const vector<array<int, 1>> &dat) {
		if(l == r) {
			tr[idx] = node(dat[l][0]);
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1, dat);
		init(mid + 1, r, 2 * idx + 2, dat);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void update(int pos, int l, int r, int idx) {
		if(l == r) {
			// DO SOMETHING
			return;
		}

		int mid = (l + r) >> 1;
		if(pos <= mid) update(pos, l, mid, 2 * idx + 1);
		else update(pos, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void update(int ql, int qr, int l, int r, int idx) {
		push(l, r, idx);

		if(ql > r || l > qr) {
			return;
		}

		if(ql <= l && r <= qr) {
			// DO SOMETHING
			push(l, r, idx);
			return;
		}

		int mid = (l + r) >> 1;
		update(ql, qr, l, mid, 2 * idx + 1);
		update(ql, qr, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}
} T;

void solve() {
	pre_dfs(1, 1, 0);
	T.init(0, (int)dat.size() - 1, 0, dat);
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	read();
	solve();
	return 0;
}

