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

struct segment_tree_min {
	struct node {
		int mn, pos;
		node() {
			mn = (int)1e9;
			pos = -1;
		} 

		node(int v, int i) {
			mn = v;
			pos = i;
		}
	};

	node merge(node a, node b) {
		node ret = a;
		if(chkmin(ret.mn, b.mn)) ret.pos = b.pos;
		return ret;
	}

	node tr[MAXN << 2];

	void init(int l, int r, int idx) {
		if(l == r) {
			tr[idx] = node();
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);
		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void update(int pos, int v, int l, int r, int idx) {
		if(l == r) {
			tr[idx] = node(v, pos);
			return;
		}

		int mid = (l + r) >> 1;
		if(pos <= mid) update(pos, v, l, mid, 2 * idx + 1);
		else update(pos, v, mid + 1, r, 2 * idx + 2);
		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	node query(int ql, int qr, int l, int r, int idx) {
		if(ql > r || qr < l) {
			return node();
		} 

		if(ql <= l && r <= qr) {
			return tr[idx];
		}

		int mid = (l + r) >> 1;
		return merge(query(ql, qr, l, mid, 2 * idx + 1),
				query(ql, qr, mid + 1, r, 2 * idx + 2));
	}
};

void read() {

}

void solve() {

}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);

	read();
	solve();
	return 0;
}

