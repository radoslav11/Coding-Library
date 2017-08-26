#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (int)1e5 + 42;
const int B = 2172;

int n, q;
int a[MAXN];

void read()
{
	cin >> n >> q;
	for(int i = 0; i < n; i++)
		cin >> a[i];
}

struct my_set
{
	int answer;
    unordered_map<int, int> cnt;
    my_set() { answer = 0; cnt.clear(); }

    void insert(int val)
    {
		int memo = ++cnt[val];
		if(memo == 1) answer++;
    }

    void erase(int val)
    {
		int memo = --cnt[val];
		if(memo == 0) answer--;
    }

    void clear()
    {
    	answer = 0;
    	cnt.clear();
    }

	int query() { return answer; }
};


int st_block[MAXN / B + 42], en_block[MAXN / B + 42], cnt_blocks = 0;
my_set st[(MAXN / B + 1) * (MAXN / B + 1) + 42], nw_st;

int query(int l, int r)
{
	int Lblock = l / B, Rblock = r / B;
	if(r != en_block[Rblock]) Rblock--;
	if(l != st_block[Lblock]) Lblock++;

	if(Rblock < Lblock)
	{
		nw_st.clear();
		for(int i = l; i <= r; i++)
			nw_st.insert(a[i]);
        return nw_st.query();
	}

	int mid = Lblock * cnt_blocks + Rblock;
    for(int i = l; i < st_block[Lblock]; i++) st[mid].insert(a[i]);
	for(int i = en_block[Rblock] + 1; i <= r; i++) st[mid].insert(a[i]);

	int answer = st[mid].query();

	for(int i = l; i < st_block[Lblock]; i++) st[mid].erase(a[i]);
	for(int i = en_block[Rblock] + 1; i <= r; i++) st[mid].erase(a[i]);

	return answer;
}

void update(int mid, int pos, int val)
{
	st[mid].erase(a[pos]);
	st[mid].insert(val);
}

void solve()
{
	for(int i = 0; i < n; i++)
	{
		if(i % B == 0) st_block[i / B] = i, cnt_blocks++;
		if(i % B == B - 1 || i == n - 1)
			en_block[i / B] = i;
	}

	for(int i = 0; i < cnt_blocks; i++)
		for(int j = i; j < cnt_blocks; j++)
		{
			int mid = i * cnt_blocks + j;
			st[mid] = my_set();
			for(int p = st_block[i]; p <= en_block[j]; p++)
				st[mid].insert(a[p]);
		}

    for(int p = 0; p < q; p++)
	{
        int type;
        cin >> type;

        if(type == 1)
		{
			int l, r;
			cin >> l >> r;
			cout << query(l, r) << endl;
		}
		else
		{
			int pos, val;
			cin >> pos >> val;

            for(int i = 0; i < cnt_blocks; i++)
				for(int j = i; j < cnt_blocks; j++)
					if(st_block[i] <= pos && pos <= en_block[j])
						update(i * cnt_blocks + j, pos, val);

			a[pos] = val;
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}
