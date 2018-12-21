#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);
const double inf = (double)1e17 + 42;

struct LiChao_max
{
	struct line
	{
		double a, b;
		line() { a = 0; b = 0; }
		line(double _a, double _b) { a = _a; b = _b; }
		double eval(int x) { return a * x + b; }
	};

	line tr[4 * MAXN];
	bool vis[4 * MAXN];
	int value[MAXN];
	int sz;

	void init(int l, int r, int idx)
	{
		if(r < l) return;

		vis[idx] = 0;
		if(l == r) return;

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);
	}

	void init(vector<int> Q)
	{
		sort(Q.begin(), Q.end());
		Q.erase(unique(Q.begin(), Q.end()), Q.end());

		sz = Q.size();
		for(int i = 0; i < (int)Q.size(); i++)
			value[i + 1] = Q[i];

		init(1, sz, 0);
	}

	void add_line(double a, double b) { line v = line(a, b); insert(v, 1, sz, 0); }
	double query(int x) { return query(x, 1, sz, 0); }

	void insert(line &v, int l, int r, int idx)
	{
		if(!vis[idx]) { tr[idx] = v; vis[idx] = true; return; }

		double trl = tr[idx].eval(value[l]), trr = tr[idx].eval(value[r]), vl = v.eval(value[l]), vr = v.eval(value[r]);

		if(trl >= vl && trr >= vr) return;
		if(trl < vl && trr < vr) { tr[idx] = v; return; }

		int mid = (l + r) >> 1;
		if(trl < vl) swap(tr[idx], v);
		if(tr[idx].eval(value[mid]) > v.eval(value[mid])) insert(v, mid + 1, r, 2 * idx + 2);
		else swap(tr[idx], v), insert(v, l, mid, 2 * idx + 1);
	}

	double query(int x, int l, int r, int idx)
	{
		if(!vis[idx]) return -inf;
		if(l == r) return tr[idx].eval(x);

		int mid = (l + r) >> 1;
		if(value[mid] >= x) return max(tr[idx].eval(x), query(x, l, mid, 2 * idx + 1));
		return max(tr[idx].eval(x), query(x, mid + 1, r, 2 * idx + 2));
	}
};

struct LiChao_min
{
	struct line
	{
		double a, b;
		line() { a = 0; b = 0; }
		line(double _a, double _b) { a = _a; b = _b; }
		double eval(int x) { return a * x + b; }
	};

	line tr[4 * MAXN];
	bool vis[4 * MAXN];
	int value[MAXN];
	int sz;

	void init(int l, int r, int idx)
	{
		if(r < l) return;

		vis[idx] = 0;
		if(l == r) return;

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);
	}

	void init(vector<int> Q)
	{
		sort(Q.begin(), Q.end());
		Q.erase(unique(Q.begin(), Q.end()), Q.end());

		sz = Q.size();
		for(int i = 0; i < (int)Q.size(); i++)
			value[i + 1] = Q[i];

		init(1, sz, 0);
	}

	void add_line(double a, double b) { line v = line(a, b); insert(v, 1, sz, 0); }
	double query(int x) { return query(x, 1, sz, 0); }

	void insert(line &v, int l, int r, int idx)
	{
		if(!vis[idx]) { tr[idx] = v; vis[idx] = true; return; }

		double trl = tr[idx].eval(value[l]), trr = tr[idx].eval(value[r]), vl = v.eval(value[l]), vr = v.eval(value[r]);

		if(trl <= vl && trr <= vr) return;
		if(trl > vl && trr > vr) { tr[idx] = v; return; }

		int mid = (l + r) >> 1;
		if(trl > vl) swap(tr[idx], v);
		if(tr[idx].eval(value[mid]) < v.eval(value[mid])) insert(v, mid + 1, r, 2 * idx + 2);
		else swap(tr[idx], v), insert(v, l, mid, 2 * idx + 1);
	}

	double query(int x, int l, int r, int idx)
	{
		if(!vis[idx]) return inf;
		if(l == r) return tr[idx].eval(x);

		int mid = (l + r) >> 1;
		if(value[mid] >= x) return min(tr[idx].eval(x), query(x, l, mid, 2 * idx + 1));
		return min(tr[idx].eval(x), query(x, mid + 1, r, 2 * idx + 2));
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
