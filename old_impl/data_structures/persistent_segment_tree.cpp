#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
const int MAXN = (1 << 20);

struct node {
    int sum;
    node *l, *r;
    node() {
        l = nullptr;
        r = nullptr;
        sum = 0;
    }
    node(int x) {
        sum = x;
        l = nullptr;
        r = nullptr;
    }
};

node* merge(node* l, node* r) {
    node* ret = new node(0);
    ret->sum = l->sum + r->sum;
    ret->l = l;
    ret->r = r;
    return ret;
}

node* init(int l, int r) {
    if(l == r) {
        return (new node(0));
    }

    int mid = (l + r) >> 1;
    return merge(init(l, mid), init(mid + 1, r));
}

node* update(int pos, int val, int l, int r, node* nd) {
    if(pos < l || pos > r) {
        return nd;
    }
    if(l == r) {
        return (new node(val));
    }

    int mid = (l + r) >> 1;
    return merge(
        update(pos, val, l, mid, nd->l), update(pos, val, mid + 1, r, nd->r)
    );
}

int query(int qL, int qR, int l, int r, node* nd) {
    if(qL <= l && r <= qR) {
        return nd->sum;
    }
    if(qL > r || qR < l) {
        return 0;
    }

    int mid = (l + r) >> 1;
    return query(qL, qR, l, mid, nd->l) + query(qL, qR, mid + 1, r, nd->r);
}

int get_kth(int k, int l, int r, node* nd) {
    if(l == r) {
        return l;
    }

    int mid = (l + r) >> 1;
    if(nd->l->sum < k) {
        return get_kth(k - nd->l->sum, mid + 1, r, nd->r);
    } else {
        return get_kth(k, l, mid, nd->l);
    }
}

void read() {}

node* t[MAXN];

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
