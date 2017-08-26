#include <bits/stdc++.h>
#define endl '\n'

#define double long double

using namespace std;
const int MAXN = (1 << 20);
const double dinf = (double)1e17;
const double eps = 0.00000000000001;

struct point
{
	double x, y;
	point() {x = 0; y = 0;}
	point(double _x, double _y)
	{
		x = _x;
		y = _y;
	}
};

bool cmp(point a, point b)
{
	if(a.x == b.x) return a.y < b.y;
	return a.x < b.x;
}

double slope(point a, point b)
{
	double deltax = b.x - a.x;
	double deltay = b.y - a.y;
	if(max(-deltax, deltax) < eps) return (((deltax < 0) ? (-1) : 1) * deltay < 0) ? (-dinf) : (dinf);

	return deltay / deltax; 
}

struct convex_hull
{
	vector<point> st;
	convex_hull() {st.clear();}
	
	void compute_hull(vector<point> vec)
	{
		sort(vec.begin(), vec.end(), cmp);
	
		for(int i = 0; i < (int)vec.size(); i++)
		{
			while(st.size() >= 2 && slope(st[st.size() - 2], vec[i]) >= slope(st[st.size() - 2], st[st.size() - 1])) 
				st.pop_back();
			st.push_back(vec[i]);
		}
		
		st.pop_back();
		int k = st.size();
	
		for(int i = vec.size() - 1; i >= 0; i--)
		{
			while(st.size() - k >= 2 && slope(vec[i], st[st.size() - 2]) >= slope(st[st.size() - 1], st[st.size() - 2]))
				st.pop_back();
			st.push_back(vec[i]);
		}

		st.pop_back();
	}
};

int n;
vector<point> a;

void read()
{
	cin >> n;

	for(int i = 0; i < n; i++)
	{
		double x, y;
		cin >> x >> y;
		a.push_back(point(x, y));
	}
}

convex_hull hull;

void solve()
{
	hull.compute_hull(a);
}	

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

