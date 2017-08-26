#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct node
{
	int sum;
	node() { sum = 0; }
	node(int _x) { sum = _x; }
};

inline node operator+(const node &l, const node &r)
{
	node ret;
	ret.sum = l.sum + r.sum;
	return ret;
}

template<class T> 
struct segment_tree
{
    int n;
    T t[2 * MAXN];

    void init(int sz)
    {
        n = sz;
        for(int i = 0; i < n; i++) t[i + n] = T();
        for(int i = n - 1; i > 0; --i)
            t[i] = t[i << 1] + t[i << 1 | 1];
    }

    void modify(int p, const T& value)
    {
        for(t[p += n] = value; p >>= 1; )
            t[p] = t[p << 1] + t[p << 1 | 1];
    }

    T query(int l, int r)
    {
        T resl, resr;
        for(l += n, r += n; l < r; l >>= 1, r >>= 1)
        {
            if(l & 1) resl = resl + t[l++];
            if(r & 1) resr = t[--r] + resr;
        }

        return resl + resr;
    }
};

void read()
{

}

segment_tree<node> t;

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
