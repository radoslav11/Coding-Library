#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int ALPH = SIGMA;

struct palindromic_tree
{
	int s[MAXN], len[MAXN], link[MAXN];
	map<char, int> to[MAXN]; 
	int n, last, psz;
 
	void clear()
	{		
		s[n++] = -1;
		link[0] = 1;
		len[1] = -1;
		psz = 2;
	}
 
	int get_link(int v)	
	{
		while(s[n - len[v] - 2] != s[n - 1]) 
			v = link[v];
		return v;
	}
 
	void add_letter(char c)	
	{
		s[n++] = c;
		last = get_link(last);
		
		if(!to[last][c])
		{
			len[sz] = len[last] + 2;
			link[sz] = to[get_link(link[last])][c];
			to[last][c] = psz++;
		}
		
		last = to[last][c];
	}
};

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

