#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct aho_corasick
{
	int is_end[MAXN], link[MAXN], psz;
	map<char, int> to[MAXN];

	void clear()
	{
		for(int i = 0; i < psz; i++)
			is_end[i] = 0, link[i] = 0, to[i].clear();
		
		psz = 1;
		is_end[0] = 1;
	}

	aho_corasick() { psz = MAXN - 2; clear(); }

	void add_word(string s)
	{
		int u = 0;
		for(char c: s)
		{
			if(!to[u].count(c)) to[u][c] = psz++;
			u = to[u][c];
		}

		is_end[u] = 1;
	}

	void push_links()
	{
		queue<int> Q;
		int u, v, j; 
		char c;
		
		Q.push(0);
		link[0] = -1;
		
		while(!Q.empty())
		{
			u = Q.front();
			Q.pop();

			for(auto it: to[u])
			{
				v = it.second;
				c = it.first;
				j = link[u];
			
				while(j != -1 && !to[j].count(c)) j = link[j];
				if(j != -1) link[v] = to[j][c];
				else link[v] = 0;
			
				Q.push(v);
			}
		}
	}
};

int n, m;
string s[MAXN];

void read()
{
	cin >> n >> m;
	for(int i = 0; i < m; i++)
		cin >> s[i];
}

aho_corasick aho;

void solve()
{
	aho.clear();
	for(int i = 0; i < m; i++)
		aho.add_word(s[i]);
	aho.push_links();
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

