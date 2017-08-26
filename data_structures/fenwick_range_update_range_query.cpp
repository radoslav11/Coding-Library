#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

template<class T>
struct fenwick_range_update
{
	int sz;
	T tr[MAXN];

	void init(int _sz)
	{
		sz = _sz + 1;
		memset(tr, 0, sizeof(tr));
	}

	T query(int idx)
	{
		T ans = 0;
		for(; idx >= 1; idx -= (idx & -idx))
			ans += tr[idx];
		return ans;
	}

	void update(int idx, T val)
	{
		if(idx <= 0) return;
		for(; idx <= sz; idx += (idx & -idx))
			tr[idx] += val;
	}

	void update(int l, int r, T val)
	{
		update(l, val);
		update(r + 1, -val);
	}
};

template<class T>
struct fenwick
{
	int sz;
	fenwick_range_update<T> t0, t1;	
	
	void init(int _sz)
	{
		sz = _sz;
		t0.init(sz + 1);
		t1.init(sz + 1);
	}

	void update(int l, int r, T val)
	{
		t0.update(l, r, val);
		t1.update(l, r, -(l - 1) * val);
		t1.update(r + 1, sz, (r - l + 1) * val);
	}

	T query(int idx) { return t0.query(idx) * idx + t1.query(idx); }
	T query(int l, int r) { return query(r) - query(l - 1); }
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

