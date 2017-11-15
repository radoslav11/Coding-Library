#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);
const int inf = (int)1e9 + 42;

struct node
{
	int mn;
	node() { mn = inf; }
	node(int val) { mn = val; }
};

node temp, broken;

node merge(node l, node r)
{
	temp.mn = min(l.mn, r.mn);
	return temp;
}

int bound_L[MAXN], bound_R[MAXN];
struct segment_tree
{
	node tr[4 * MAXN];

	void init(int l, int r, int idx)
	{
		bound_L[idx] = l;
		bound_R[idx] = r;
		if(l == r)
		{
			tr[idx] = node(inf);
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
			tr[idx].mn = val;
			return;
		}

		int mid = (l + r) >> 1;
		update(pos, val, l, mid, 2 * idx + 1);
		update(pos, val, mid + 1, r, 2 * idx + 2);

		tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
	}

	void get_nodes(int qL, int qR, int l, int r, int idx, vector<int> &li)
	{
		if(l > qR || r < qL) return;
		if(qL <= l && r <= qR)
		{
			li.push_back(idx);
			return;
		}

		int mid = (l + r) >> 1;
		get_nodes(qL, qR, l, mid, 2 * idx + 1, li);
		get_nodes(qL, qR, mid + 1, r, 2 * idx + 2, li);
	}

	int get_right(int l, int r, int idx, int X)
	{
		if(l == r) return l;
		int mid = (l + r) >> 1;
		if(tr[2 * idx + 1].mn <= X) return get_right(l, mid, 2 * idx + 1, X);
		else return get_right(mid + 1, r, 2 * idx + 2, X);
	}

	int get_left(int l, int r, int idx, int X)
	{
		if(l == r) return l;
		int mid = (l + r) >> 1;
		if(tr[2 * idx + 2].mn <= X) return get_left(mid + 1, r, 2 * idx + 2, X);
		else return get_left(l, mid, 2 * idx + 1, X);
	}
};

int n;
segment_tree t;

int get_left(int pos, int val)
{
	vector<int> li;
	t.get_nodes(1, pos, 1, n, 0, li);
	reverse(li.begin(), li.end());

	for(int it: li)
		if(t.tr[it].mn <= val) 
			return t.get_left(bound_L[it], bound_R[it], it, val);

	return 0;
}

int get_right(int pos, int val)
{
	vector<int> li;
	t.get_nodes(pos, n, 1, n, 0, li);

	for(int it: li)
		if(t.tr[it].mn <= val) 
			return t.get_right(bound_L[it], bound_R[it], it, val);


	return n + 1;
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

