#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

template<class T>
struct clever_set
{
	map<pair<int, int>, T> value;

	void init(int n) { value[{n, 1}] = (T)0; }

	vector<pair<pair<int, int>, T> > add(int l, int r, T val)
	{
		auto bg = value.lower_bound({l, 0})->first;
		if(bg.second != l)
		{
			T val = value[bg];
			value.erase(bg);
			value[{l - 1, bg.second}] = val;
			value[{bg.first, l}] = val;
		}

		auto en = value.lower_bound({r, 0})->first;
		if(en.first != r)
		{
			T val = value[en];
			value.erase(en);
			value[{en.first, r + 1}] = val;
			value[{r, en.second}] = val;
		}
		
		vector<pair<pair<int, int>, T> > ret;
		auto itt = value.lower_bound({l, 0});
		while(true)
		{
			if(itt == value.end() || itt->first.first > r) break;
			ret.push_back({{itt->first.second, itt->first.first}, itt->second});
			++itt;
		}

		for(auto it: ret) 
			value.erase({it.first.second, it.first.first});

		value[{r, l}] = val;
		return ret;
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

