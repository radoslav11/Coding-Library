#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

random_device rd;
mt19937 mt(rd());

struct persistent_treap
{
	struct node
	{
		int sz, mx, val, lazy;		
		node *l, *r;
		node() { sz = 0; mx = 0; lazy = 0; val = 0; l = nullptr; r = nullptr; }
		node(int _val) { lazy = 0; val = _val; mx = val; sz = 1; l = nullptr; r = nullptr; }
	};

	typedef node* pnode;

	pnode copy_node(pnode prv)
	{
		if(!prv) return nullptr;

		pnode ret = new node();
		ret->l = prv->l;
		ret->r = prv->r;
		ret->val = prv->val;
		ret->sz = prv->sz;
		ret->mx = prv->mx;
		ret->lazy = prv->lazy;
		return ret;
	}

	int size(pnode v) { return v ? v->sz : 0; }
	void update_size(pnode &v) { if(v) v->sz = size(v->l) + size(v->r) + 1; }
	void reset(pnode &v) { if(v) v->mx = v->val; }
	bool hey(int a, int b) { return (int)mt() % (a + b) < a; }

	void push(pnode &t)
	{
		if(t && t->lazy)
		{
			t->mx += t->lazy;
			t->val += t->lazy;
			if(t->l) t->l = copy_node(t->l), t->l->lazy += t->lazy;
			if(t->r) t->r = copy_node(t->r), t->r->lazy += t->lazy;
			t->lazy = 0;
		}
	}

	void combine(pnode &v, pnode l, pnode r) 
	{
		if(!l) { v = r; return; }
		if(!r) { v = l; return; }

		v->mx = max(l->mx, r->mx);
	}

	void operation(pnode &v) 
	{  
		if(!v) return;

		push(v->l);
		push(v->r);

		reset(v);
		combine(v, v->l, v);
		combine(v, v, v->r);
	}

	void merge(pnode &t, pnode l, pnode r)
	{	
		push(l), push(r);

		if(!l) { t = copy_node(r); return; }
		if(!r) { t = copy_node(l); return; }

		if(hey(size(l), size(r)))
		{
			t = copy_node(l);
			merge(t->r, l->r, r);
		}
		else	
		{
			t = copy_node(r);
			merge(t->l, l, r->l);
		}

		update_size(t);
		operation(t);
	}

	void split(pnode t, pnode &l, pnode &r, int k, int add = 0)
	{
		push(t);

		if(!t) { l = nullptr; r = nullptr; return; }

		int idx = add + size(t->l);
		if(idx <= k)
		{
			l = copy_node(t);
			split(t->r, l->r, r, k, idx + 1);
			update_size(l);
			operation(l);
		}
		else
		{
			r = copy_node(t);
			split(t->l, l, r->l, k, add);
			update_size(r);
			operation(r);
		}

		update_size(t);
		operation(t);
	}

	pnode root;
	persistent_treap() { root = nullptr; }

	void insert(int pos, int val)
	{
		if(!root)
		{
			pnode nw = new node(val);
			root = nw;
			return;
		}

		pnode l, r, nw = new node(val);

		split(root, l, r, pos - 1);
		merge(l, l, nw);
		merge(root, l, r);
	}

	int query_max(int qL, int qR)
	{
		pnode l, r, mid;

		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);

		int ret = mid ? mid->mx : -1;

		merge(r, mid, r);
		merge(root, l, r);

		return ret;
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

