#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct segment_tree
{
	int ord[4 * MAXN], sum[4 * MAXN];

	void init(int l, int r, int idx) 
	{  
		if(l == r)
		{
			ord[idx] = 0;
			sum[idx] = 0; 
			return;
		}

		int mid = (l + r) >> 1;
		init(l, mid, 2 * idx + 1);
		init(mid + 1, r, 2 * idx + 2);
	
		if(ord[idx]) sum[idx] = (r - l + 1);
		else sum[idx] = sum[2 * idx + 1] + sum[2 * idx + 2];
	}

	void update(int qL, int qR, int val, int l, int r, int idx)
	{
		if(r < qL || l > qR)
			return;

		if(qL <= l && r <= qR)
		{
			ord[idx] += val;
			if(ord[idx] > 0) sum[idx] = (r - l + 1);
			else sum[idx] = sum[2 * idx + 1] + sum[2 * idx + 2];
			return;
		}
	
		int mid = (l + r) >> 1;
		update(qL, qR, val, l, mid, 2 * idx + 1);
		update(qL, qR, val, mid + 1, r, 2 * idx + 2);

		if(ord[idx] > 0) sum[idx] = (r - l + 1);
		else sum[idx] = sum[2 * idx + 1] + sum[2 * idx + 2];
	}
};

void read()
{

}	

segment_tree t;

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

