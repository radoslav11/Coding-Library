#include <bits/stdc++.h>
#define endl '\n'

#define int long long
#define pow mypow

using namespace std;
const int MAXN = (1 << 20);
const int mod = (int)1e9 + 7;
int INV2;

typedef vector<int> polynomial;

int pow(int x, int p)
{
	if(p == 0) return 1;

	if(p % 2 == 0)
	{
		int d = pow(x, p / 2);
		return (d * d) % mod;
	}

	return (pow(x, p - 1) * x) % mod;
}

int inv(int x, int mod) { return pow(x, mod - 2); }

void fft(polynomial &a, int low, int high)
{
	if(high == low + 1)
		return;

	int len = (high - low) >> 1, mid = low + len;
	fft(a, low, mid);
	fft(a, mid, high);

	for(int i = low; i < mid; i++)
	{
		int x1 = a[i] % mod;
		int x2 = a[i + len] % mod;

		a[i] = (x1 - x2 + mod) % mod;
		a[i + len] = (x1 + x2) % mod;
	}
}

void inv_fft(polynomial &a, int low, int high)
{
	if(high == low + 1)
		return;

	int len = (high - low) >> 1, mid = low + len;

	for(int i = low; i < mid; i++)
	{
		int y1 = a[i] % mod;
		int y2 = a[i + len] % mod;

		a[i] = ((y1 + y2) * INV2) % mod;
		a[i + len] = ((y2 - y1 + mod) * INV2) % mod;
	}
	
	inv_fft(a, low, mid);
	inv_fft(a, mid, high);
}

void read()
{
	
}

void solve()
{
	INV2 = inv(2, mod);
	cout << INV2 << endl;
}

#undef int
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

