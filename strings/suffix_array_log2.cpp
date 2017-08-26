#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

int n;
string s;

void read()
{
	cin >> s;
	n = s.size();
}

int lcp[MAXN], sa[MAXN], rnk[MAXN], idx[MAXN], temp[MAXN], gap;

bool cmp(int i, int j)
{
	if(idx[i] != idx[j]) return idx[i] < idx[j]; i += gap; j += gap;
	return (i < n && j < n) ? idx[i] < idx[j] : i > j;
}

void build_suffix_array()
{
	for(int i = 0; i < n; i++)
	{
		sa[i] = i;
		idx[i] = s[i];
	}

	for(gap = 1; ; gap = gap * 2)
	{
		sort(sa, sa + n, cmp);

		for(int i = 0; i < n - 1; i++) 
		{
			temp[i + 1] = temp[i];
			temp[i + 1] += cmp(sa[i], sa[i + 1]);
		}
		
		for(int i = 0; i < n; i++) 
			idx[sa[i]] = temp[i];
		
		if(temp[n - 1] == n - 1) break;
	}
}

void build_lcp()
{
	int k = 0;

	for(int i = 0; i < n; i++)
		rnk[sa[i]] = i;

	for(int i = 0; i < n; i++, k ? k-- : 0)
	{
		if(rnk[i] == n - 1)
		{
			k = 0;
			continue;
		}

		int j = sa[rnk[i] + 1];	
		while(i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
		lcp[rnk[i]] = k;
	}	
}

int dp[MAXN][20];
int logr[MAXN];

void sparse_table()
{
	for(int i = 0; i < n; i++)
		dp[i][0] = lcp[i];

	for(int lg = 1; (1 << lg) <= n; lg++)
		for(int i = 0; i + (1 << lg) < n; i++)
			dp[i][lg] = min(dp[i][lg - 1], dp[i + (1 << (lg - 1))][lg - 1]);

	logr[0] = -1;
	for(int i = 1; i <= n; i++)
		logr[i] = logr[i / 2] + 1;
}

int LCP(int l, int r)
{
	if(l > r) return n - sa[l];
	int len = r - l + 1, lg = logr[len];
	return min(dp[l][lg], dp[r - (1 << lg) + 1][lg]);
}

void prepare()
{
	build_suffix_array();
	build_lcp();
	sparse_table();
}

void solve()
{
	prepare();
}

int main()
{
	read();
	solve();
	return 0;
}

