#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 mt(rd());

struct node {
    int sz, prior, value;
    node *l, *r;
    node() {
        value = 0;
        sz = 0;
        prior = 0;
        l = nullptr;
        r = nullptr;
    }
    node(int v) {
        value = v;
        sz = 1;
        prior = mt();
        l = nullptr;
        r = nullptr;
    }
};

typedef node *pnode;

inline int size(pnode v) { return v ? v->sz : 0; }

void pull(pnode &v) {
    if(!v) {
        return;
    }
    v->sz = size(v->l) + size(v->r) + 1;
}

void merge(pnode &t, pnode l, pnode r) {
    if(!l) {
        t = r;
        return;
    }
    if(!r) {
        t = l;
        return;
    }

    if(l->prior > r->prior) {
        merge(l->r, l->r, r), t = l;
    } else {
        merge(r->l, l, r->l), t = r;
    }

    pull(t);
}

void split(pnode t, pnode &l, pnode &r, int k) {
    if(!t) {
        l = nullptr;
        r = nullptr;
        return;
    }

    if(t->value <= k) {
        split(t->r, t->r, r, k), l = t;
    } else {
        split(t->l, l, t->l, k), r = t;
    }

    pull(t);
}

void merge_op(pnode &t, pnode l, pnode r) {
    if(!l) {
        t = r;
        return;
    }
    if(!r) {
        t = l;
        return;
    }

    if(l->prior < r->prior) {
        swap(l, r);
    }

    pnode L, R;
    split(r, L, R, l->value - mt() % 2);
    merge_op(l->r, l->r, R);
    merge_op(l->l, L, l->l);

    t = l;
    pull(t);
}

void split_sz(pnode t, pnode &l, pnode &r, int k, int add = 0) {
    if(!t) {
        l = nullptr;
        r = nullptr;
        return;
    }

    int idx = add + size(t->l);
    if(idx <= k) {
        split_sz(t->r, t->r, r, k, idx + 1), l = t;
    } else {
        split_sz(t->l, l, t->l, k, add), r = t;
    }

    pull(t);
}

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
