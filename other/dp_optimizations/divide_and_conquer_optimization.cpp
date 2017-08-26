#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (int)4e3 + 42;
const int inf = (int)1e9 + 42;

void Read(int &x);

int n, m;

void read()
{

}

int dp[MAXN][MAXN], cost[MAXN][MAXN];

void rec(int l, int r, int opt_l, int opt_r, int k)
{
	if(r < l) return;

	if(l == r)
	{
		dp[l][k] = inf;
		for(int i = opt_l; i <= min(opt_r, l); i++)
			if(dp[l][k] > dp[i - 1][k - 1] + cost[i][l])
				dp[l][k] = dp[i - 1][k - 1] + cost[i][l];

		return;
	}

	int mid = (l + r) >> 1, copt = opt_l;

	dp[mid][k] = inf;
	for(int i = opt_l; i <= min(mid, opt_r); i++)
		if(dp[mid][k] > dp[i - 1][k - 1] + cost[i][mid])
		{
			dp[mid][k] = dp[i - 1][k - 1] + cost[i][mid];
			copt = i;
		}

	rec(l, mid - 1, opt_l, copt, k);
	rec(mid + 1, r, copt, opt_r, k);
}

void solve()
{
	for(int i = 0; i <= n; i++) dp[i][0] = inf;
	for(int i = 0; i <= m; i++) dp[0][i] = 0;
	for(int k = 1; k <= m; k++)
		rec(1, n, 1, n, k);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

#define maxl 100000
char sir[maxl];
int pos_ind = 0;

void Next() { if(++pos_ind == maxl) fread(sir, 1, maxl, stdin), pos_ind = 0;  }
void Read(int &x)
{
		for(; sir[pos_ind] < '0' || sir[pos_ind] > '9'; Next());
			for(x = 0; sir[pos_ind] >= '0' && sir[pos_ind] <= '9'; Next())
						x = x * 10 + sir[pos_ind] - '0';

}
