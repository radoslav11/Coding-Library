#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2> inline void chkmax(T &x, const T2 &y) { if(x < y) x = y; }
template<class T, class T2> inline void chkmin(T &x, const T2 &y) { if(x > y) x = y; }
const int MAXN = (1 << 20);

template<class T>
struct monotonous_queue_max
{
	int len, current_position;
	deque<pair<T, int> > Q;

	monotonous_queue_max() { Q.clear(); current_position = 0; len = 0; }

	void init(int _l)
	{
		len = _l;
		current_position = 0;
		Q.clear();
	}

	void push_back(T x)
	{
		while(!Q.empty() && current_position - Q.front().second >= len) Q.pop_front();
		while(!Q.empty() && x >= Q.back().first) Q.pop_back();
		Q.push_back({x, current_position++});
	}

	T query()
	{
		if(Q.empty()) return -(T)1e9;
		return Q.front().first;
	}
};

template<class T>
struct monotonous_queue_min
{
	int len, current_position;
	deque<pair<T, int> > Q;

	monotonous_queue_min() { Q.clear(); current_position = 0; len = 0; }

	void init(int _l)
	{
		len = _l;
		current_position = 0;
		Q.clear();
	}

	void push_back(T x)
	{
		while(!Q.empty() && current_position - Q.front().second >= len) Q.pop_front();
		while(!Q.empty() && x <= Q.back().first) Q.pop_back();
		Q.push_back({x, current_position++});
	}

	T query()
	{
		if(Q.empty()) return (T)1e9;
		return Q.front().first;
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

