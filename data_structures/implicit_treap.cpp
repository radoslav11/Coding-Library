#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct implicit_treap
{
	struct node
	{
		int val, sz, priority, lazy, rev, sum;
		node *l, *r, *par;

		node() { lazy = 0; rev = 0; val = 0; sz = 0; priority = 0; l = NULL; r = NULL; par = NULL;}
		node(int _val)
		{
			val = _val;
			sum = _val;
			rev = 0;
			lazy = 0;
			sz = 1;
			priority = rand();

			l = NULL;
			r = NULL;
			par = NULL;
		}
	};

	typedef node* pnode;

	pnode root;
	map<int, pnode> position;
	
	void clear()
	{
		root = NULL;
		position.clear();
	}

	implicit_treap() { clear(); }

	int size(pnode p) { return p ? p->sz : 0; }
	void update_size(pnode &p) { if(p) p->sz = size(p->l) + size(p->r) + 1; }
	
	void update_parent(pnode &p)
	{
		if(!p) return;
		if(p->l) p->l->par = p;
		if(p->r) p->r->par = p;
	}

	void push(pnode &p)
	{
		if(!p) return;		
		p->sum += size(p) * p->lazy;
		p->val += p->lazy;

		if(p->rev) swap(p->l, p->r);
		
		if(p->l) { p->l->lazy += p->lazy; p->l->rev ^= p->rev; }
		if(p->r) { p->r->lazy += p->lazy; p->r->rev ^= p->rev; }
	
		p->lazy = 0;
		p->rev = 0;
	}

	void reset(pnode &t) { if(t) t->sum = t->val; }

	void combine(pnode &t, pnode l, pnode r)
	{
		if(!l) { t = r; return; }
		if(!r) { t = l; return; }
		t->sum = l->sum + r->sum;
	}

	void operation(pnode &t)
	{
		if(!t) return;

		reset(t);
		push(t->l);
		push(t->r);

		combine(t, t->l, t);
		combine(t, t, t->r);
	}

	void split(pnode t, pnode &l, pnode &r, int k, int add = 0)
	{
		if(t == NULL) { l = NULL; r = NULL; return; }
		push(t);

		int idx	= add + size(t->l);
		if(idx <= k) 
			split(t->r, t->r, r, k, idx + 1), l = t;
		else
			split(t->l, l, t->l, k, add), r = t;

		update_parent(t);
		update_size(t);
		operation(t);
	}

	void merge(pnode &t, pnode l, pnode r)
	{
		push(l);
		push(r);

		if(!l) { t = r; return; }
		if(!r) { t = l; return; }

		if(l->priority > r->priority)
			merge(l->r, l->r, r), t = l;
		else 
			merge(r->l, l, r->l), t = r;

		update_parent(t);
		update_size(t);
		operation(t);
	}

	void insert(int pos, int val)
	{
		if(root == NULL)
		{
			pnode to_add = new node(val);
			root = to_add;
			position[val] = root;
			return;
		}

		pnode l, r, mid;
		mid = new node(val);
		position[val] = mid;

		split(root, l, r, pos - 1);
		merge(l, l, mid);
		merge(root, l, r);
	}

	void erase(int qL, int qR)
	{
		pnode l, r, mid;
		
		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);
		merge(root, l, r);
	}

	int query(int qL, int qR)
	{
		pnode l, r, mid;

		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);

		int answer = mid->sum;
	
		merge(r, mid, r);
		merge(root, l, r);

		return answer;
	}

	void update(int qL, int qR, int val)
	{
		pnode l, r, mid;

		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);

		mid->lazy += val;

		merge(r, mid, r);
		merge(root, l, r);
	}

	void reverse(int qL, int qR)
	{
		pnode l, r, mid;

		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);

		mid->rev ^= 1;
		merge(r, mid, r);
		merge(root, l, r);
	}

	void cyclic_shift(int qL, int qR, int k)
	{
		if(qL == qR) return;
		k %= (qR - qL + 1);

		pnode l, r, mid, fh, sh;
		split(root, l, r, qL - 1);
		split(r, mid, r, qR - qL);

		split(mid, fh, sh, (qR - qL + 1) - k - 1);
		merge(mid, sh, fh);

		merge(r, mid, r);
		merge(root, l, r);
	}

	int get_pos(pnode curr, pnode son = nullptr)
	{
		if(!son)
			if(curr == root) return size(curr->l);
			else return size(curr->l) + get_pos(curr->par, curr);

		if(curr == root)
			if(son == curr->l) return 0;
			else return size(curr->l) + 1;

		if(curr->l == son) return get_pos(curr->par, curr);
		else return get_pos(curr->par, curr) + size(curr->l) + 1;
	}

	int get_pos(int value) { return get_pos(position[value]); }
};

int n;
int a[MAXN];

void read()
{
	cin >> n;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}

implicit_treap t;

void solve()
{
	t.clear();
	for(int i = 0; i < n; i++)
		t.insert(i, a[i]);

}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

