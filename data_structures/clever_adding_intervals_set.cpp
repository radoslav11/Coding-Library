#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

struct clever_set
{
	set<pair<int, int> > st;
	map<pair<int, int>, int> value;

	void init(int n) { st.insert({n, 1}); value[{n, 1}] = -1; }

	vector<pair<pair<int, int>, int> > add(int l, int r, int val)
	{
		auto bg = *st.lower_bound({l, 0});
		if(bg.second != l)
		{
			int val = value[bg];
			st.erase(bg);
			value.erase(bg);

			value[{l - 1, bg.second}] = val;
			st.insert({l - 1, bg.second});

			value[{bg.first, l}] = val;
			st.insert({bg.first, l});
		}

		auto en = *st.lower_bound({r, 0});
		if(en.first != r)
		{
			int val = value[en];
			st.erase(en);
			value.erase(en);

			value[{en.first, r + 1}] = val;
			st.insert({en.first, r + 1});

			value[{r, en.second}] = val;
			st.insert({r, en.second});
		}
		
		vector<pair<pair<int, int>, int> > ret;
		auto it = st.lower_bound({l, 0});
		while(true)
		{
			if(it == st.end() || it->first > r) break;
			ret.push_back({{it->second, it->first}, value[*it]});
			++it;
		}

		for(auto it: ret) 
		{
			st.erase({it.first.second, it.first.first});
			value.erase({it.first.second, it.first.first});
		}

		st.insert({r, l});
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

