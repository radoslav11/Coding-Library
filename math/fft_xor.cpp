#include <bits/stdc++.h>
#define endl '\n'

#define int long long

using namespace std;
const int MAXN = (1 << 20);

typedef vector<int> polynomial; 

void fft(polynomial &a, int low, int high)
{
	if(low == high - 1)
		return;

	int len = (high - low) / 2, mid = low + len;
	fft(a, low, mid);
	fft(a, mid, high);

	for(int i = low; i < mid; i++)
	{
		int x1 = a[i];
		int x2 = a[i + len];

		a[i] = (x1 - x2);
		a[i + len] = (x1 + x2);
	}
}

void inv_fft(polynomial &a, int low, int high)
{
	if(low == high - 1)
		return;

	int len = (high - low) / 2, mid = low + len;

	for(int i = low; i < mid; i++)
	{
		int y1 = a[i];
		int y2 = a[i + len];

		a[i] = (y1 + y2) / 2;
		a[i + len] = (y2 - y1) / 2;
	}
	
	inv_fft(a, low, mid);
	inv_fft(a, mid, high);
}

void read()
{
	
}

void solve()
{

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

