#include <bits/stdc++.h>
#define endl '\n'

#define int long long

using namespace std;
const int MAXN = (1 << 21);
const int bound = (int)1e6 + 42;
const int inf = (int)1e9 + 42;

struct edge
{
	int x, y1, y2, val;
	edge() {x = -1; y1 = -1; y2 = -1; val = -1;}
	edge(int _x, int _y1, int _y2, int _val)
	{
		x = _x;
		y1 = _y1;
		y2 = _y2;
		val = _val;
	}
};

bool operator<(const edge &e1, const edge &e2)
{
	return e1.x < e2.x;
}

int n;
vector<edge> e;

void read()
{
	cin >> n;
	for(int i = 0; i < n; i++)
	{
		int mnx, mny, mxx, mxy;
		cin >> mnx >> mny >> mxx >> mxy;

		if(mxx < mnx) swap(mxx, mnx);
		if(mxy < mny) swap(mxy, mny);
		mnx--;
		mny--;

		e.push_back(edge(mnx, mny, mxy, 1));
		e.push_back(edge(mxx, mny, mxy, -1));
	}	
}

int valy[MAXN];
vector<int> comy;
map<int, int> id_y;
int sz_y;

void compress()
{
	for(int i = 0; i < e.size(); i++)
	{
		comy.push_back(e[i].y1);
		comy.push_back(e[i].y2);
	}

	sort(comy.begin(), comy.end());
	id_y[comy[0]] = 1;

	for(int i = 1; i < comy.size(); i++)
		if(comy[i] != comy[i - 1])
			id_y[comy[i]] = id_y[comy[i - 1]] + 1ll;

	for(int i = 0; i < comy.size(); i++)
		valy[id_y[comy[i]]] = comy[i];

	for(int i = 0; i < e.size(); i++)
	{
		e[i].y1 = id_y[e[i].y1];
		e[i].y2 = id_y[e[i].y2];
	}

	sz_y = id_y.size();
}

struct segment_tree
{
	struct node
	{
		int lazy, ans;
		node() {lazy = 0, ans = 0;}
		node(int v) { ans = v; lazy = 0;}
	};

	node tr[4 * MAXN];

	void update(int qL, int qR, int val, int l, int r, int idx)
	{
		if(qL <= l && r <= qR)
		{
			tr[idx].lazy += val;
			if(tr[idx].lazy == 0) tr[idx].ans = tr[2 * idx + 1].ans + tr[2 * idx + 2].ans; 
			else tr[idx].ans = valy[r] - valy[l];
			return;
		}

		if(qL > r || qR < l)
			return;
	
		if(l + 1 >= r)
			return;

		int mid = (l + r) >> 1;
		update(qL, qR, val, l, mid, 2 * idx + 1);
		update(qL, qR, val, mid, r, 2 * idx + 2);

		if(tr[idx].lazy == 0) tr[idx].ans = tr[2 * idx + 1].ans + tr[2 * idx + 2].ans;
		else tr[idx].ans = valy[r] - valy[l];
	}
};

segment_tree t;

inline int mabs(int x) { return x < 0 ? -x : x; }

int rectangle_union()
{
	compress();
	sort(e.begin(), e.end());

	int ans = 0, last = -inf;
	for(int i = 0; i < e.size(); i++)
	{
		ans += mabs(e[i].x - last) * (long long)t.tr[0].ans;
		t.update(e[i].y1, e[i].y2, e[i].val, 1, sz_y, 0);
		last = e[i].x;
	}

	return ans;
}

void solve()
{
	cout << rectangle_union() << endl;	
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

