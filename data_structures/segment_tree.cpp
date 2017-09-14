#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 18);

int n, m;
int a[MAXN];

struct node
{
	int sum;

	node() {sum = 0;}
	node(int val)
	{
		sum = val;
	}
};

node temp, broken;

node merge(node l, node r)
{
	temp.sum = l.sum + r.sum;
	return temp;
}

struct segment_tree
{
	node tr[4 * MAXN];

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

	void update(int pos, int val, int l, int r, int idx)
	{
		if(l > pos || r < pos)
			return;

		if(l == r && l == pos)
		{
			tr[idx].sum += val;
			return;
		}

		int mid = (l + r) >> 1;
		update(pos, val, l, mid, 2 * idx + 1);
		update(pos, val, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	node query(int qL, int qR, int l, int r, int idx)
	{
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
	t.init(0, n - 1, 0);

	for(int i = 0; i < m; i++)
	{
		int l, r;
		cin >> l >> r;
		cout << t.query(l, r, 0, n - 1, 0).sum << endl;
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

