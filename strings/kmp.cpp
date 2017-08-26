#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

string str, pat;

void read()
{
	cin >> str;
	cin >> pat;
}

vector<int> failure_function(string p)
{
	int sz = p.size();
	vector<int> f;
	f.assign(sz + 1, 0);

	int j = 0;
	f[0] = 0;

	for(int i = 1; i < sz; i++)
	{
		while(j >= 0 && p[i] != p[j])
		{
			if(j >= 1) j = f[j - 1];
			else j = -1;
		}

		j++;
		f[i] = j;
	}

	return f;
}

void match(vector<int> f, string p, string s)
{
	int psz = p.size(), sz = s.size();

	int j = 0;
	for(int i = 0; i < sz; i++)
	{
		while(j >= 0 && p[j] != s[i])
			if(j >= 1) j = f[j - 1];
			else j = -1;

		j++;
		if(j == psz)
		{
			j = f[j - 1];
			cout << "Found pattern in [" << i - psz + 1 << "; " << i << "]" << endl;
		}
	}
}

vector<int> f;

void solve()
{
	f = failure_function(pat);
	match(f, pat, str);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

