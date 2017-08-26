#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct trie
{
	int psz, cnt[MAXN], len[MAXN];
	map<char, int> to[MAXN];	

	trie() { psz = MAXN - 1; }

	void clear() { for(int i = 0; i < psz; i++) cnt[i] = 0, len[i] = 0, to[i].clear(); psz = 1; }

	void add(string s)
	{
		int u = 0;
		for(char c: s)	
		{
			if(!to[u].count(c)) len[psz] = len[u] + 1, to[u][c] = psz++;
			u = to[u][c]; cnt[u]++;
		}
	}	
	
	void del(string s)
	{
		int u = 0;
		for(char c: s)	
		{
			if(!to[u].count(c)) return;
			u = to[u][c]; cnt[u]--;
		}
	}

	int count(string s)
	{
		int u = 0;
		for(char c: s)
		{
			if(!to[u].count(c)) return 0;
			u = to[u][c]; 
		}

		return cnt[u];
	}
};

trie t;

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

