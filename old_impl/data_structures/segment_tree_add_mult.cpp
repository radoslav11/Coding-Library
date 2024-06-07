#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);
const int mod = (int)1e9 + 7;

struct node
{
	int sum, lazy, mult;

	node() {sum = 0; mult = 1; lazy = 0;}
	node(int val)
	{
		sum = val;
		lazy = 0;
		mult = 1;
	}
};

node temp, broken;

node merge(node l, node r)
{
	temp.sum = (l.sum + r.sum) % mod;
	temp.lazy = 0;
	temp.mult = 1;
	return temp;
}

struct segment_tree
{
	node tr[4 * MAXN];

	void push(int l, int r, int idx)
	{
		if(tr[idx].mult != 1)
		{
			tr[idx].sum = (tr[idx].sum * 1ll * tr[idx].mult) % mod;
			tr[idx].sum %= mod;

			if(l != r)
			{
				tr[2 * idx + 1].mult = (tr[2 * idx + 1].mult * 1ll * tr[idx].mult) % mod;
				tr[2 * idx + 2].mult = (tr[2 * idx + 2].mult * 1ll * tr[idx].mult) % mod;
				tr[2 * idx + 1].lazy = (tr[2 * idx + 1].lazy * 1ll * tr[idx].mult) % mod;
				tr[2 * idx + 2].lazy = (tr[2 * idx + 2].lazy * 1ll * tr[idx].mult) % mod;
			}

			tr[idx].mult = 1;
		}

		if(tr[idx].lazy)
		{
			tr[idx].sum += ((r - l + 1) * 1ll * tr[idx].lazy) % mod;
			tr[idx].sum %= mod;

			if(l != r)
			{
				tr[2 * idx + 1].lazy = (tr[2 * idx + 1].lazy + tr[idx].lazy) % mod;
				tr[2 * idx + 2].lazy = (tr[2 * idx + 2].lazy + tr[idx].lazy) % mod;
			}

			tr[idx].lazy = 0;
		}
	}

	void init(int l, int r, int idx)
	{
		if(l == r)
		{
			tr[idx] = node();
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void mult(int qL, int qR, int x, int l, int r, int idx)
	{
		push(l, r, idx);
		if(qL > r || l > qR)
			return;

		if(qL <= l && r <= qR)
		{
			tr[idx].mult = (tr[idx].mult * 1ll * x) % mod;
			push(l, r, idx);
			return;
		}		

		int mid = (l + r) >> 1;
		mult(qL, qR, x, l, mid, 2 * idx + 1);
		mult(qL, qR, x, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}
	
	void add(int qL, int qR, int x, int l, int r, int idx)
	{
		push(l, r, idx);
		if(qL > r || l > qR)
			return;

		if(qL <= l && r <= qR)
		{
			tr[idx].lazy = (tr[idx].lazy + x) % mod;
			push(l, r, idx);
			return;
		}		

		int mid = (l + r) >> 1;
		add(qL, qR, x, l, mid, 2 * idx + 1);
		add(qL, qR, x, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	node query(int qL, int qR, int l, int r, int idx)
	{
		push(l, r, idx);

		if(l > qR || r < qL)
			return broken;

		if(qL <= l && r <= qR)
			return tr[idx];

		int mid = (l + r) >> 1;
		return merge(query(qL, qR, l, mid, 2 * idx + 1), query(qL, qR, mid + 1, r, 2 * idx + 2));
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

