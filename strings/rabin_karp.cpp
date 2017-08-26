#include <bits/stdc++.h>
#define endl '\n'

#define int long long

using namespace std;
const int MAXN = (1 << 20);

struct rabin_karp
{
	int mod, base, sz, len;
	int h[MAXN], base_pow[MAXN];

	void init(string s, int _base, int _mod)
	{
		mod = _mod;
		base = _base;
		sz = s.size();
	
		h[0] = 0;	
		for(int i = 1; i <= sz; i++)
			h[i] = (h[i - 1] * base + s[i - 1]) % mod;
		
		base_pow[0] = 1;
		for(int i = 1; i <= sz; i++)
			base_pow[i] = (base_pow[i - 1] * base) % mod;
	}

	int get_hash(int l, int r)
	{
		len = r - l + 1;
		return (h[r] - (h[l - 1] * base_pow[len]) % mod + mod) % mod;
	}
};

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

