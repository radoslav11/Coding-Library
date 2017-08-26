#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int bound = (int)(1e6) + 42;

template<typename T>
struct fenwick_2d
{
	int sz; 
	unordered_map<int, T> t[MAXN];

	void init(int _sz) {sz = _sz;}

	void update_row(int row, int idx, T val)
	{
		if(idx == 0) return;
		for(; idx <= sz; idx += (idx & -idx))
			t[row][idx] = min(t[row][idx], val);
	}

	T query_row(int row, int idx)
	{
		T ans = 0;
		for(; idx >= 1; idx -= (idx & -idx))
			ans = min(t[row][idx], ans);
		
		return ans;
	}

	void update(int row, int col, T val)
	{
		if(col == 0) return;
		for(; row <= sz; row += (row & -row))	
			update_row(row, col, val);
	}
	
	T query(int row, int col)
	{
		T ans = 0;
		for(; row >= 1; row -= (row & -row))
			ans = min(query_row(row, col), ans);
		
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
