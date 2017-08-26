#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct query
{
	int l, r, block;
	int idx;

	query() {l = -1; r = -1; block = -1; idx = -1;}
	query(int _l, int _r, int i)
	{
		l = _l;
		r = _r;
		idx = i;
	}
};

bool cmp(query a, query b)
{
	if(a.block != b.block) return a.block < b.block;
	return a.r < b.r;
}

int n, m, len, sq, item;

void read()
{
	cin >> n >> m;

}

int ans, answer[MAXN];

void add(int idx)
{

}

void del(int idx)
{	

}		

query q[MAXN];

void solve()
{
	for(int i = 0; i < m; i++)
	{
		int l, r;
		cin >> l >> r;
		q[i] = query(l - 1, r - 1, i);
	}

	sq = sqrt(n);
	for(int i = 0; i < m; i++) q[i].block = q[i].l / sq;
	sort(q, q + m, cmp);

	int l = q[0].l, r = q[0].r;
	for(int i = l; i <= r; i++)
		add(i);

	for(int i = 0; i < m; i++)
	{
		while(l < q[i].l) del(l++);
		while(l > q[i].l) add(--l);
		while(r < q[i].r) add(++r);
		while(r > q[i].r) del(r--);
		
		answer[q[i].idx] = ans;
	}

	for(int i = 0; i < m; i++)
		cout << answer[i] << endl;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

