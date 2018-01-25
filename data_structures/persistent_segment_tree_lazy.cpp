#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

int n, q;
int a[MAXN];

struct node
{
	int64_t lazy, sum;
	node *l, *r;

	node() { lazy = sum = 0; l = r = nullptr; }
	node(int64_t val) { lazy = 0; sum = val; l = r = nullptr; }
};

typedef node* pnode;

pnode merge(pnode a, pnode b)
{
	pnode ret = new node();
	
	ret->sum = a->sum + b->sum;
	ret->l = a;
	ret->r = b;
	
	return ret;
}

pnode cop(pnode b)
{
	if(!b) return nullptr;

	pnode ret = new node();
	ret->sum = b->sum;
	ret->lazy = b->lazy;
	
	ret->l = b->l;
	ret->r = b->r;
	return ret;
}

void push(int l, int r, pnode &ver)
{
	if(!ver->lazy) return;

	ver->sum += (r - l + 1) * 1ll * ver->lazy;
	
	ver->l = cop(ver->l);
	ver->r = cop(ver->r);

	if(l != r)
	{
		ver->l->lazy += ver->lazy;
		ver->r->lazy += ver->lazy;
	}

	ver->lazy = 0;
}

pnode init(int l, int r)
{
	if(l == r) return new node(a[l]);	
	int mid = (l + r) >> 1;
	return merge(init(l, mid), init(mid + 1, r));
}

pnode update(int qL, int qR, int val, int l, int r, pnode prv)
{
	push(l, r, prv);

	if(qL <= l && r <= qR) 
	{
		pnode ret = cop(prv);
		ret->lazy += val;
		push(l, r, ret);
		return ret;
	}

	if(qL > r || qR < l) return prv;

	int mid = (l + r) >> 1;
	return merge(update(qL, qR, val, l, mid, prv->l), update(qL, qR, val, mid + 1, r, prv->r));
}

int64_t query(int qL, int qR, int l, int r, pnode nd)
{
	push(l, r, nd);
	if(qL > r || qR < l) return 0;
	if(qL <= l && r <= qR) return nd->sum;

	int mid = (l + r) >> 1;
	return query(qL, qR, l, mid, nd->l) + query(qL, qR, mid + 1, r, nd->r);
}

void read()
{
	cin >> n >> q;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}

pnode root[MAXN];

void solve()
{
	root[1] = init(0, n - 1);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

