#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct node
{
	int sum;
	node() { sum = 0; }
	node(int _x) { sum = _x; }
};

inline node operator+(const node &l, const node &r)
{
	node ret;
	ret.sum = l.sum + r.sum;
	return ret;
}

template<class T> 
struct segment_tree
{	
	T tr[4 * MAXN];

	void init(int l, int r, int idx)
	{
		if(l == r) 
		{
			tr[idx] = T();
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);

		tr[idx] = tr[2 * idx + 1] + tr[2 * idx + 2];
	}

	void update(int pos, int new_val, int l, int r, int idx)
	{
		if(pos < l || pos > r)
			return;

		if(l == r && pos == l) 
		{
			tr[idx] = T(new_val);
			return;
		}

		int mid = (l + r) >> 1;
		update(pos, new_val, l, mid, 2 * idx + 1);
		update(pos, new_val, mid + 1, r, 2 * idx + 2);

		tr[idx] = tr[2 * idx + 1] + tr[2 * idx + 2];
	}

	T query(int ql, int qr, int l, int r, int idx)
	{
		if(ql > r || l > qr) return T();
		if(ql <= l && r <= qr) return tr[idx];
	
		int mid = (l + r) >> 1;
		return query(ql, qr, l, mid, 2 * idx + 1) + query(ql, qr, mid + 1, r, 2 * idx + 2);
	}	
};

segment_tree<node> t;

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

