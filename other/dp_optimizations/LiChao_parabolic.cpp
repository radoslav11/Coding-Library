#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int inf = (int)1e9 + 42;

template<class T>
struct LiChao_parabolic_min {
	struct Parabol {
		T a, b, c;
		Parabol() {a = 0; b = 0; c = inf;}
		Parabol(T a, T b, T c) : a(a), b(b), c(c) {}
		Parabol(const Parabol& rhs) {
			a = rhs.a;
			b = rhs.b;
			c = rhs.c;
		}
		T query(T x) {
			return a * x * x + b * x + c;
		}
	};
	struct Node {
		Parabol prb;
		Node *l, *r;
		Node() : l(0), r(0) {}
		T query(T x) {
			return prb.query(x);
		}
	};

	Node* rt;
	T offset;
	vector<Parabol> mem;

	LiChao_parabolic_min() : rt(0), offset(0) {}

	Node* upd(Node* p, int l, int r, int L, int R, Parabol prb) {
		int M = (L + R) >> 1;
		if (l > R || r < L) return p;
		if (!p) p = new Node();
		if (l <= L && r >= R) {
			if (prb.query(L) >= p->query(L) && prb.query(R) >= p->query(R)) {
				return p;
			}
			if (prb.query(L) <= p->query(L) && prb.query(R) <= p->query(R)) {
				p->prb = prb;
				return p;
			}
			if (prb.query(L) >= p->query(L) && prb.query(M) >= p->query(M)) {
				p->r = upd(p->r, l, r, M + 1, R, prb);
				return p;
			}
			if (prb.query(L) <= p->query(L) && prb.query(M) <= p->query(M)) {
				p->r = upd(p->r, l, r, M + 1, R, p->prb);
				p->prb = prb;
				return p;
			}
			if (prb.query(M + 1) >= p->query(M + 1) && prb.query(R) >= p->query(R)) {
				p->l = upd(p->l, l, r, L, M, prb);
				return p;
			}
			if (prb.query(M + 1) <= p->query(M + 1) && prb.query(R) <= p->query(R)) {
				p->l = upd(p->l, l, r, L, M, p->prb);
				p->prb = prb;
				return p;
			}
			return p;
		}
		else if (L < R) {
			p->l = upd(p->l, l, r, L, (L + R) >> 1, prb);
			p->r = upd(p->r, l, r, ((L + R) >> 1) + 1, R, prb);
		}
	}
	T query(Node* p, int i, int L, int R) {
		if (!p) return inf;
		if (i < L || i > R) return inf;
		T res = inf;
		res = min(res, p->query(i));
		if (L < R) {
			res = min(res, query(p->l, i, L, (L + R) >> 1));
			res = min(res, query(p->r, i, ((L + R) >> 1) + 1, R));
		}
		return res;
	}
	void upd(T a, T b, T c) {
		mem.push_back(Parabol(a, b, c));
		rt = upd(rt, 0, 100005, 0, 100005, Parabol(a, b, c));
	}
	T query(int i) {
		return query(rt, i, 0, 100005);
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
