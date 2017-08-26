#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (int)1e5 + 42;
const int MAXLOG = 20;

struct suffix_array 
{
	pair<pair<int,int>, int> L[MAXN]; 
	int P[MAXLOG + 1][MAXN], n, stp, cnt, sa[MAXN];

	suffix_array() { n = 0; stp = 0; cnt = 0; }
	suffix_array(const string& s) : n(s.size()) 
	{ 
		for(int i = 0; i < n; i++)
			 P[0][i] = s[i];
		
		sa[0] = 0; 
		for(stp = 1, cnt = 1; cnt < n; stp++, cnt <<= 1) 
		{
			for(int i = 0; i < n; i++)
				L[i] = {{P[stp - 1][i], i + cnt < n ? P[stp - 1][i + cnt] : -1}, i};
			
			sort(L, L + n);
			for(int i = 0; i < n; i++)
				P[stp][L[i].second] = i > 0 && L[i].first == L[i - 1].first ? P[stp][L[i - 1].second] : i;
		}
		
		for(int i = 0; i < n; i++) 
			sa[i] = L[i].second;
	}
	
	int &operator[](int idx) { return sa[idx]; }	

	int lcp(int x, int y) 
	{ 
		int k, ret = 0;
		if (x == y) return n - x;
		for (k = stp - 1; k >= 0 && x < n && y < n; k--)
			if (P[k][x] == P[k][y])
				x += (1 << k), y += (1 << k), ret += (1 << k);
		
		return ret;
	}
};

string s;

void read()
{	
	cin >> s;
}

void solve()
{
	suffix_array sa(s);
	for(int idx = 0; idx < (int)s.size(); idx++)
		cout << sa[idx] << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

