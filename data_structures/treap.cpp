#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

random_device rd;
mt19937_64 mt(rd());

struct node
{
	int sz, prior, value;
	node *l, *r;
	node() { value = 0; sz = 0; prior = 0; l = nullptr; r = nullptr; }
	node(int v) { value = v; sz = 1; prior = rand(); l = nullptr; r = nullptr; }
};

typedef node* pnode;

int size(pnode v) { return v ? v->sz : 0; }
void update_size(pnode &v) { if(v) v->sz = size(v->l) + size(v->r) + 1; }

void merge(pnode &t, pnode l, pnode r)
{
	if(!l) { t = r; return; }
	if(!r) { t = l; return; }

	if(l->prior > r->prior)
		merge(l->r, l->r, r), t = l;
	else
		merge(r->l, l, r->l), t = r;

	update_size(t);
}

void split(pnode t, pnode &l, pnode &r, int k)
{
	if(!t) { l = nullptr; r = nullptr; return; }

	if(t->value <= k)
		split(t->r, t->r, r, k), l = t;
	else
		split(t->l, l, t->l, k), r = t;

	update_size(t);
}

void merge_op(pnode &t, pnode l, pnode r)
{
	if(!l) { t = r; return;  }
	if(!r) { t = l; return;  }

	if(l->prior < r->prior)
		swap(l, r);

	pnode L, R;
	split(r, L, R, l->value - mt() % 2);
	merge_op(l->r, l->r, R);
	merge_op(l->l, L, l->l);

	t = l;
	update_size(t);
}

void split_sz(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
	if(!t) { l = nullptr; r = nullptr; return; }

	int idx = add + size(t->l);
	if(idx <= k)
		split_sz(t->r, t->r, r, k, idx + 1), l = t;
	else
		split_sz(t->l, l, t->l, k, add), r = t;

	update_size(t);
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

