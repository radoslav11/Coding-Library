#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

template <class T> struct fenwick {
	int sz;
	T tr[MAXN];

	void init(int n) {
		sz = n + 1;
		memset(tr, 0, sizeof(tr));
	}

	T query(int idx) {
		T ans = 0;
		for(; idx >= 1; idx -= (idx & -idx)) {
			ans += tr[idx];
		}

		return ans;
	}

	void update(int idx, T val) {
		if(idx <= 0) return;
		for(; idx <= sz; idx += (idx & -idx)) {
			tr[idx] += val;	
		}
	}

	T query(int l, int r) { 
		return query(r) - query(l - 1); 
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

