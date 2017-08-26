#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const int64_t inf = (int64_t)1e18 + 42;

struct convex_hull_trick_min
{
	struct line
	{
		int64_t m, b;
		bool is_query;
		double line_x;

		line() {m = 0; b = 0; line_x = 0; is_query = 0; }
		line(int64_t _m, int64_t _b) {m = _m; b = _b; line_x = 0; is_query = 0; }

		bool operator<(const line &l2) const
		{
			if(!is_query && !l2.is_query) return m > l2.m;
			else return line_x < l2.line_x;
		}

		int64_t value_at(int64_t x) const { return m * x + b; }
	};

	bool parallel(const line &l1, const line &l2) { return l1.m == l2.m; }

	double intersect_x(const line &l1, const line &l2)
	{
		if(parallel(l1, l2)) return (double)inf;
		return (double)(l1.b - l2.b) / (double)(l2.m - l1.m);
	}

	set<line> hull;
	void clear() { hull.clear(); }
	void init() { clear(); }

	bool has_next(set<line>::iterator it) { return it != hull.end() && next(it) != hull.end(); }
	bool has_prev(set<line>::iterator it) { return it != hull.end() && it != hull.begin(); }

	bool irrelevant(const line &l1, const line &l2, const line &l3) { return intersect_x(l1, l3) <= intersect_x(l1, l2); }
	bool irrelevant(set<line>::iterator it) { return has_prev(it) && has_next(it) && irrelevant(*prev(it), *it, *next(it)); }

	set<line>::iterator update_border(set<line>::iterator it)
	{
		line tmp(*it);

		double val;
		if(has_next(it)) val = intersect_x(*it, *next(it));
		else val = inf;

		it = hull.erase(it);
		tmp.line_x = val;
		it = hull.insert(it, tmp);

		return it;
	}

	void add_line(int64_t m, int64_t b)
	{
		line curr_line = line(m, b);
		set<line>::iterator it = hull.lower_bound(curr_line);

		if(it != hull.end() && it->m == m)
		{
			if(it->b > b) it = hull.erase(it);
			else return;
		}

		it = hull.insert(it, curr_line);
		if(irrelevant(it))
		{
			hull.erase(it);
			return;
		}

		while(has_prev(it) && irrelevant(prev(it))) hull.erase(prev(it));
		while(has_next(it) && irrelevant(next(it))) hull.erase(next(it));

		it = update_border(it);
		if(has_prev(it)) update_border(prev(it));
		if(has_next(it)) update_border(next(it));
	}

	int64_t query(int64_t x)
	{
		line to_find;
		to_find.line_x = x;
		to_find.is_query = 1;

		set<line>::iterator best_line = hull.lower_bound(to_find);
		if(best_line == hull.end())
			return inf;

		return best_line->value_at(x);
	}
};

void read()
{

}

convex_hull_trick_min cht;

void solve()
{
	cht.init();

}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

