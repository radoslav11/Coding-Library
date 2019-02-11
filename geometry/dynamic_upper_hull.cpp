#include <bits/stdc++.h>
#define endl '\n'

#define SZ(x) ((int)x.size())
#define ALL(V) V.begin(), V.end()
#define L_B lower_bound
#define U_B upper_bound
#define pb push_back

using namespace std;
template<class T, class T1> int chkmin(T &x, const T1 &y) { return x > y ? x = y, 1 : 0; }
template<class T, class T1> int chkmax(T &x, const T1 &y) { return x < y ? x = y, 1 : 0; }
const int MAXN = (int)5e4 + 42;

struct PT
{
    int x, y;
    PT() { x = y = 0; }
    PT(int _x, int _y) { x = _x; y = _y; }

    int64_t operator&(const PT &other) { return x * 1ll * other.y - y * 1ll * other.x; }
    PT operator+(const PT &other) const { return PT(x + other.x, y + other.y); }
    PT operator-(const PT &other) const { return PT(x - other.x, y - other.y); }
};

random_device rd;
mt19937 mt(rd());

int64_t eval(PT L, PT R) { return (L.y + R.y) * 1ll * (R.x - L.x); }

struct node
{
    PT pnt;
    int prior, sz;
    int64_t answer;

    node *l, *r, *leftmost, *rightmost, *prv;
    node() { sz = 0; answer = prior = 0; leftmost = rightmost = prv = l = r = nullptr; pnt = PT(); }
    node(PT a) {leftmost = rightmost = this; sz = 1; answer = 0; pnt = a; prior = mt(); prv = l = r = nullptr; }
};

using pnode = node*;

inline int size(pnode &t) { return t ? t->sz : 0; }

void pull(pnode &t)
{
    if(!t) return;

    t->answer = 0;
    t->sz = size(t->l) + size(t->r) + 1;
    t->leftmost = t->rightmost = t;
    t->prv = nullptr;

    if(t->l)
    {
        t->answer += t->l->answer;
        t->answer += eval(t->l->rightmost->pnt, t->pnt);
        t->prv = t->l->rightmost;
        t->leftmost = t->l->leftmost;
    }

    if(t->r)
    {
        t->answer += t->r->answer;
        t->answer += eval(t->pnt, t->r->leftmost->pnt);
        t->r->leftmost->prv = t;
        t->rightmost = t->r->rightmost;
    }
}

void merge(pnode &t, pnode l, pnode r)
{
    if(!l) { t = r; return; }
    if(!r) { t = l; return; }

    if(l->prior > r->prior)
        merge(l->r, l->r, r), t = l;
    else
        merge(r->l, l, r->l), t = r;

    pull(t);
}

void split_x(pnode t, pnode &l, pnode &r, int k)
{
    if(!t) { l = r = nullptr; return; }

    if(t->pnt.x <= k)
        split_x(t->r, t->r, r, k), l = t;
    else
        split_x(t->l, l, t->l, k), r = t;

    pull(t);
}

void split_sz(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
    if(!t) { l = r = nullptr; return; }

    int idx = size(t->l) + add;
    if(idx <= k)
        split_sz(t->r, t->r, r, k, idx + 1), l = t;
    else
        split_sz(t->l, l, t->l, k, add), r = t;

    pull(t);
}

int q, T;
int tin[MAXN], tout[MAXN], n = 0;
PT p[MAXN];

void read()
{
    cin >> q >> T;
    for(int i = 0; i < q; i++)
    {
        char type;
        cin >> type;

        if(type == '+')
        {
            tin[n++] = i;
			tout[n - 1] = q - 1;

            int _p, _t;
            cin >> _p >> _t;
            p[n - 1] = PT(_t, _p);
        }
        else
        {
            int idx = 0;
            cin >> idx;
            tout[idx] = i - 1;
        }
    }
}

pnode root;
vector<int> li[4 * MAXN];
int64_t answer[MAXN];

void add(int ql, int qr, int i, int l, int r, int idx)
{
    if(r < ql || qr < l)
        return;

    if(ql <= l && r <= qr)
    {
        li[idx].pb(i);
        return;
    }

    int mid = (l + r) >> 1;
    add(ql, qr, i, l, mid, 2 * idx + 1);
    add(ql, qr, i, mid + 1, r, 2 * idx + 2);
}

int64_t cww(const PT &a, const PT &b, const PT &c) { return (b - a) & (c - a); }

int find_pref(pnode t, PT R)
{
	if(!t) return 0;
	if(!t->prv || cww(t->prv->pnt, t->pnt, R) < 0)
		return max(t->pnt.x, find_pref(t->r, R));
	else
		return find_pref(t->l, R);
}

int find_suff(pnode t, PT L)
{
	if(!t) return 0;
	if(t->prv == nullptr)
		return find_suff(t->r, L);
	else 
	{
		if(cww(L, t->prv->pnt, t->pnt) >= 0)
			return max(t->prv->pnt.x, find_suff(t->r, L));
		else
			return find_suff(t->l, L);
	}
}


pair<bool, pnode> add(pnode &t, PT pnt)
{
	pnode l, r, nw = new node(pnt);
    split_x(t, l, r, pnt.x);

    if(cww(l->rightmost->pnt, pnt, r->leftmost->pnt) >= 0)
    {
		merge(t, l, r);
		return {false, nullptr};
	}

    int good_pref = find_pref(l, pnt), bad_pref = find_suff(r, pnt);
    pnode rem_l, rem_r;

    split_x(l, l, rem_l, good_pref);
    split_x(r, rem_r, r, bad_pref);

	//cout << "Adding " << pnt.x << " " << pnt.y << " -> " << good_pref << " " << bad_pref << endl << flush;
	//cout << "Sizes: " << size(l) << " " << size(r) << endl << flush;

	assert(l != nullptr);
	assert(r != nullptr);
    
	pnode ret;
    merge(t, l, nw);
    merge(t, t, r);
    merge(ret, rem_l, rem_r);

    return {true, ret};
}

void solve(int l, int r, int idx)
{
    stack<pair<int, pnode> > ST;
    for(int i: li[idx])
    {
        pair<bool, pnode> q = add(root, p[i]);
        if(q.first == true) ST.push({p[i].x, q.second});
	}

    if(l == r)
		answer[l] = root->answer;
	else
    {
        int mid = (l + r) >> 1;
        solve(l, mid, 2 * idx + 1);
        solve(mid + 1, r, 2 * idx + 2);
    }

    while(!ST.empty())
    {
        auto mid = ST.top();
        ST.pop();

        pnode L, R, dummy;
        split_x(root, L, R, mid.first);
        split_x(L, L, dummy, mid.first - 1);

        merge(root, L, mid.second);
        merge(root, root, R);
    }
}

void solve()
{
    for(int i = 0; i < n; i++) add(tin[i], tout[i], i, 0, q - 1, 0);

    root = nullptr;
    merge(root, root, new node(PT(0, 0)));
    merge(root, root, new node(PT(T, 0)));
    solve(0, q - 1, 0);

    for(int i = 0; i < q; i++)
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
