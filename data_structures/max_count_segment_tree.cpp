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

struct count_max_segment_tree {
	struct node {
		int mx, cnt, lazy;
		node() { mx = -(int)1e9, cnt = 0, lazy = 0; }
		node(int v) {
			mx = v;
			cnt = 1;
			lazy = 0;
		}
	};

	node merge(node a, node b) {
		node ret = node();
		ret.mx = max(a.mx, b.mx);
		
		ret.cnt = 0;
		if(ret.mx == a.mx) ret.cnt += a.cnt;
		if(ret.mx == b.mx) ret.cnt += b.cnt;
		
		return ret;
	}

	node tr[MAXN << 2];

	void push(int l, int r, int idx) {
		if(tr[idx].lazy) {
			tr[idx].mx += tr[idx].lazy;
			if(l != r) {
				tr[2 * idx + 1].lazy += tr[idx].lazy;
				tr[2 * idx + 2].lazy += tr[idx].lazy;
			}
			tr[idx].lazy = 0;
		}
	}

	void init(int l, int r, int idx) {
		if(l == r) {
			tr[idx] = node(-l);
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);
	
		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void add(int ql, int qr, int v, int l, int r, int idx) {
		push(l, r, idx);

		if(qr < l || ql > r) {
			return;
		}

		if(ql <= l && r <= qr) {
			tr[idx].lazy += v;
			push(l, r, idx);
			return;
		}

		int mid = (l + r) >> 1;
		add(ql, qr, v, l, mid, 2 * idx + 1);
		add(ql, qr, v, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	node query(int ql, int qr, int l, int r, int idx) {
		push(l, r, idx);

		if(qr < l || ql > r) {
			return node();
		}

		if(ql <= l && r <= qr) {
			return tr[idx];
		}

		int mid = (l + r) >> 1;
		return merge(query(ql, qr, l, mid, 2 * idx + 1),
					 query(ql, qr, mid + 1, r, 2 * idx + 2));
	}

	void walk(int l, int r, int idx) {
		push(l, r, idx);

		if(l == r) {
			cout << l << ": " << "{" << tr[idx].mx << " " << tr[idx].cnt << " lazy=" << tr[idx].lazy <<"}" << " ";
			return;
		}

		int mid = (l + r) >> 1;
		walk(l, mid, 2 * idx + 1);
		walk(mid + 1, r, 2 * idx + 2);
	
		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
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

