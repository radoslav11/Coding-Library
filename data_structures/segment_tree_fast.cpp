#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct node
{
    int mx;
    node() { mx = -1e9; }
    node(int val) { mx = val; }
};

node temp;

node merge(node l, node r)
{
    temp.mx = max(l.mx, r.mx);
    return temp;
}

struct segment_tree
{
    int n;
    node t[2 * MAXN];

    void init(int sz)
    {
        n = sz;
        for(int i = 0; i < n; i++) t[i + n] = node();
        for(int i = n - 1; i > 0; --i)
            t[i] = merge(t[i << 1], t[i << 1 | 1]);
    }

    void modify(int p, const node& value)
    {
        for(t[p += n] = value; p >>= 1; )
            t[p] = merge(t[p << 1], t[p << 1 | 1]);
    }

    node query(int l, int r)
    {
        node resl, resr;
        for(l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if(l & 1) resl = merge(resl, t[l++]);
            if(r & 1) resr = merge(t[--r], resr);
        }

        return merge(resl, resr);
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
