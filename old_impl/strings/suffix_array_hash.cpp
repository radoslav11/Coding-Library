#include <bits/stdc++.h>
#define endl '\n'

#define int long long

using namespace std;
const int MAXN = (1 << 20);

struct rabin_karp
{
	int mod, base, sz;
	vector<int> h, base_pow;

	void init(string s, int _mod, int _base)
	{
		mod = _mod;
		base = _base;
		sz = s.size();
	
		h.assign(sz + 1, 0);
		base_pow.assign(sz + 1, 0);
	
		for(int i = 1; i <= sz; i++)
			h[i] = (h[i - 1] * base + s[i - 1]) % mod;
		
		base_pow[0] = 1;
		for(int i = 1; i <= sz; i++)
			base_pow[i] = (base_pow[i - 1] * base) % mod;
	}

	int get_hash(int l, int r)
	{
		int len = r - l + 1;
		return (h[r] - (h[l - 1] * base_pow[len]) % mod + mod) % mod;
	}
};

rabin_karp fir, sec;
string str;
int sz;

bool cmp(int i, int j)
{
	int l = 1, r = min(sz - i, sz - j), mid, ans = 0;

	while(l <= r)
	{
		mid = (l + r) >> 1;

		if(fir.get_hash(i + 1, i + mid) == fir.get_hash(j + 1, j + mid) && 
	   	   sec.get_hash(i + 1, i + mid) == sec.get_hash(j + 1, j + mid))
		{	
			l = mid + 1;
			ans = mid;
		}
		else
			r = mid - 1;
	}	

	if(ans == min(sz - i, sz - j)) return (sz - i) < (sz - j);
	return (str[i + ans] < str[j + ans]);
}

vector<int> kasai(string s, vector<int> sa)
{
	sz = s.size();

	vector<int> lcp, pos;
	lcp.assign(sz, 0);
	pos.assign(sz, 0);

	for(int i = 0; i < sz; i++)
		pos[sa[i]] = i;

	int k = 0;
	for(int i = 0; i < sz; i++, k ? k-- : 0)
	{
		if(pos[i] == sz - 1)
		{
			k = 0;
			continue;
		}

		int j = sa[pos[i] + 1];
		while(i + k < sz && j + k < sz && s[i + k] == s[j + k]) k++;
		lcp[pos[i]] = k;
	}

	return lcp;
}

vector<int> build(string s)
{
	vector<int> _sa;
	sz = s.size();
	str = s;

	fir.init(str, (int)(1e9 + 7), 131);
	sec.init(str, (int)(1e9 + 9), 137);
		
	_sa.assign(sz, 0);
	for(int i = 0; i < sz; i++)
		_sa[i] = i;

	stable_sort(_sa.begin(), _sa.end(), cmp);
	return _sa;
}		

int n;
string a;

void read()
{
	cin >> a;
	n = a.size();
}

vector<int> sa, lcp;

void solve()
{
	sa = build(a);
	lcp = kasai(a, sa);
	
	cout << "arr:  ";
	for(int i = 0; i < n; i++)
		cout << sa[i] << " ";
	cout << endl;

	cout << "lcp:  ";
	for(int i = 0; i < n - 1; i++)
		cout << lcp[i] << " ";
	cout << endl;
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

