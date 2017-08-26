#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

template<class T>
struct fenwick_xor
{
	T tr[MAXN];
	int sz;

	void init(int _sz) { memset(tr, 0, sizeof(tr)); sz = _sz; }

	void update(int idx, T val)
	{
		if(idx <= 0) return;
		for(; idx <= sz; idx += (idx & -idx))
			tr[idx] ^= val;
	}

	T query(int idx)
	{
		T ans = 0;
		for(; idx >= 1; idx -= (idx & -idx))
			ans ^= tr[idx];
		return ans;
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

