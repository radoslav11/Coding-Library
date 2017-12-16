#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct suffix_automaton
{
	map<char, int> to[MAXN];
	int len[MAXN], link[MAXN];
	int last, psz = 0;

	void add_letter(char c)
	{
		int p = last, cl, q;
		if(to[p].count(c))
		{
  			q = to[p][c];
			if(len[q] == len[p] + 1)
			{
				last = q;
				return;
			}

			cl = psz++;
			len[cl] = len[p] + 1;
			to[cl] = to[q];
			link[cl] = link[q];
			link[q] = cl;
			last = cl;
	
			for(; to[p][c] == q; p = link[p])
				to[p][c] = cl;
			
			return;
		}

		last = psz++;
		len[last] = len[p] + 1;

		for(; to[p][c] == 0; p = link[p])
			to[p][c] = last;
	
		if(to[p][c] == last) 
		{
			link[last] = p;
			return;
		}

		q = to[p][c];
		if(len[q] == len[p] + 1)
		{
			link[last] = q;
			return;
		}

		cl = psz++;
		len[cl] = len[p] + 1;
		to[cl] = to[q];
		link[cl] = link[q];
		link[q] = cl;
		link[last] = cl;

		for(; to[p][c] == q; p = link[p])
			to[p][c] = cl;
	}

	void clear()
	{
		for(int i = 0; i < psz; i++)
			len[i] = 0, link[i] = 0, to[i].clear();
		psz = 1;
		last = 0;
	}

	void init(string s)
	{
		clear();
		for(int i = 0; i < (int)s.size(); i++)
			add_letter(s[i]);
	}
	
	suffix_automaton() {psz = 0; clear();}
};

void read()
{
	
}

suffix_automaton sa;

void solve()
{
	sa.clear();
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

