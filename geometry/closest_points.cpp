#include <bits/stdc++.h>
#define endl '\n'

#define int long long

using namespace std;
const int MAXN = (1 << 20);
const double eps = 0.000001;
const int inf = (int)1e17 + 42;

struct point
{
	int x, y;

	point() {x = 0; y = 0;}
	point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
};

bool cmpbyx(point a, point b)
{
	if(a.x == b.x) return a.y < b.y;
	return a.x < b.x;
}

bool cmpbyy(point a, point b)
{
	if(a.y == b.y) return a.x < b.x;
	return a.y < b.y;
}

int n;
int a[MAXN];

void read()
{
	cin >> n;
	for(int i = 1; i <= n; i++)
		cin >> a[i];
}

vector<point> p;
int dist(point p_i, point p_j) { return (p_i.x - p_j.x) * (p_i.x - p_j.x) + (p_i.y - p_j.y) * (p_i.y - p_j.y); }

int rec(int l, int r)
{
	if(l == r) return inf;

	int ret, mid = (l + r) >> 1;
	double d;
	ret = min(rec(l, mid), rec(mid + 1, r));
	d = sqrt(ret);

	vector<point> c;
	for(int i = l; i <= r; i++)
		if(abs(p[i].x - p[mid].x) <= d)
			c.push_back(p[i]);

	sort(c.begin(), c.end(), cmpbyy);

	for(int i = 0; i < c.size(); i++)
		for(int j = i + 1; j < c.size() && (c[j].y - c[i].y) <= d; j++)
		{
			ret = min(ret, dist(c[i], c[j]));
			d = sqrt(ret);
		}
		
	return ret;
}

void solve()
{
	a[0] = 0;
	for(int i = 1; i <= n; i++)
		a[i] += a[i - 1];

	for(int i = 1; i <= n; i++)
		p.push_back(point(i, a[i]));

	sort(p.begin(), p.end(), cmpbyx);

	int ans = rec(0, n - 1);
	cout << ans << endl;
}

#undef int
int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

