#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int64_t mod = (int64_t)1e9 + 7;

int64_t pow(int64_t base, int64_t p, int64_t MOD)
{
	if(p == 0) return 1;
	if(p % 2 == 0) { int64_t d = pow(base, p / 2, MOD); return (d * d) % MOD; }
	return (pow(base, p - 1, MOD) * base) % MOD;
}

int64_t inv(int64_t x, int64_t MOD) { return pow(x, MOD - 2, MOD); }

vector<int64_t> gauss(vector<vector<int64_t>> &a, int64_t MOD)
{
	int n = a.size(), m = a[0].size() - 1;

	for(int i = 0; i < n; i++)
		for(int j = 0; j <= m; j++)
			a[i][j] = (a[i][j] % MOD + MOD) % MOD;

	vector<int> where(m, -1);
	for(int col = 0, row = 0; col < m && row < n; col++)
    {
    	int sel = row;
        for(int i = row; i < n; i++)
        	if(a[i][col] > a[sel][col])
        		sel = i;

		if(a[sel][col] == 0) { where[col] = -1; continue; }

        for(int i = col; i <= m; i++)
			swap(a[sel][i], a[row][i]);
		where[col] = row;

		int64_t c_inv = inv(a[row][col], MOD);
		for(int i = 0; i < n; i++)
			if(i != row)
			{
				if(a[i][col] == 0) continue;
            	int64_t c = (a[i][col] * c_inv) % MOD;
            	for(int j = 0; j <= m; j++)
                    a[i][j] = (a[i][j] - c * a[row][j] % MOD + MOD) % MOD;
			}

		row++;
    }

    vector<int64_t> ans(m, 0);
    for(int i = 0; i < m; i++)
        if(where[i] != -1)
			ans[i] = (a[where[i]][m] * inv(a[where[i]][i], MOD)) % MOD;

    for(int i = 0; i < n; i++)
	{
		int64_t sum = a[i][m] % MOD;
		for(int j = 0; j < m; j++)
			sum = (sum + MOD - (ans[j] * a[i][j]) % MOD) % MOD;

		if(sum != 0) return vector<int64_t>();
	}

	return ans;
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
