#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 18);

int n, m;
int a[MAXN];

struct node
{
	int sum, lazy;

	node() {sum = 0; lazy = 0;}
	node(int val)
	{
		sum = val;
		lazy = 0;
	}
};

node temp, broken;

node merge(node l, node r)
{
	temp.sum = l.sum + r.sum;
	temp.lazy = 0;
	return temp;
}

struct segment_tree
{
	node tr[4 * MAXN];

	void push(int l, int r, int idx)
	{
		if(tr[idx].lazy)
		{
			tr[idx].sum += (r - l + 1) * tr[idx].lazy;

			if(l != r)
			{
				tr[2 * idx + 1].lazy += tr[idx].lazy;
				tr[2 * idx + 2].lazy += tr[idx].lazy;
			}

			tr[idx].lazy = 0;
		}
	}

	void init(int l, int r, int idx)
	{
		if(l == r)
		{
			tr[idx] = node(a[l]);
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void update(int qL, int qR, int val, int l, int r, int idx)
	{
		push(l, r, idx);

		if(qL > r || l > qR)
			return;

		if(qL <= l && r <= qR)
		{
			tr[idx].lazy += val;
			push(l, r, idx);
			return;
		}

		int mid = (l + r) >> 1;
		update(qL, qR, val, l, mid, 2 * idx + 1);
		update(qL, qR, val, mid + 1, r, 2 * idx + 2);

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
	cin >> n >> m;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}

segment_tree t;

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

