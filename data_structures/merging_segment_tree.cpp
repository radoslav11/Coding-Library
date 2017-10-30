#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

struct node
{
	int sz;
	node *l, *r;

	node() { sz = 0; l = r = nullptr; }
	node(int _s) { sz = _s; l = r = nullptr; }
};

typedef node* pnode; 

inline int size(pnode p) { return p ? p->sz : 0; }

inline void update_size(pnode &p)
{
	if(!p) return;
	p->sz = 1 + size(p->l) + size(p->r);
}

pnode init(int val, int l, int r)
{
	pnode ret = new node(1);
	if(l == r) return ret;

	int mid = (l + r) >> 1;
	if(val <= mid) ret->l = init(val, l, mid);
	else ret->r = init(val, mid + 1, r);

	return ret;
}

pnode merge(pnode l, pnode r)
{
	if(!l) return r;
	if(!r) return l;

	l->l = merge(l->l, r->l);
	l->r = merge(l->r, r->r);
	l->sz += r->sz;

	return l;
}

pair<pnode, pnode> split(pnode &t, int k)
{
	if(!t) return {nullptr, nullptr};
	
	pair<pnode, pnode> ret, tmp;
	ret.second = new node();

	int L = size(t->l);
	if(L < k)	
	{
		tmp = split(t->r, k - L);
		ret.second = tmp.second;
		ret.first = t;
		ret.first->r = tmp.first;
	}
	else
	{
		tmp = split(t->l, k);
		ret.first = tmp.first;
		ret.second = t;
		ret.second->l = tmp.second;
	}

	update_size(ret.first);
	update_size(ret.second);
	return ret;
}

int kth(pnode t, int k, int l, int r)
{
	if(!t) return -1;
	if(l == r) return l;

	int mid = (l + r) >> 1, L = size(t->l);
	if(L < k) return kth(t->r, k - L, l, mid);
	else return kth(t->l, k, mid + 1, r);	
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
