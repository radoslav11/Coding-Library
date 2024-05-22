#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 mt(rd());

struct implicit_treap {
    struct node {
        int sz, mx, val, prior;
        node *l, *r;
        node() {
            sz = 0;
            mx = 0;
            val = 0;
            prior = 0;
            l = nullptr;
            r = nullptr;
        }
        node(int _val) {
            val = _val;
            mx = val;
            sz = 1;
            prior = mt();
            l = nullptr;
            r = nullptr;
        }
    };

    typedef node *pnode;

    int size(pnode v) { return v ? v->sz : 0; }
    void update_size(pnode &v) {
        if(v) {
            v->sz = size(v->l) + size(v->r) + 1;
        }
    }
    void reset(pnode &v) {
        if(v) {
            v->mx = v->val;
        }
    }

    void combine(pnode &v, pnode l, pnode r) {
        if(!l) {
            v = r;
            return;
        }
        if(!r) {
            v = l;
            return;
        }

        v->mx = max(l->mx, r->mx);
    }

    void operation(pnode &v) {
        if(!v) {
            return;
        }

        reset(v);
        combine(v, v->l, v);
        combine(v, v, v->r);
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

        update_size(t);
        operation(t);
    }

    void split(pnode t, pnode &l, pnode &r, int k, int add = 0) {
        if(!t) {
            l = nullptr;
            r = nullptr;
            return;
        }

        int idx = add + size(t->l);
        if(idx <= k) {
            split(t->r, t->r, r, k, idx + 1), l = t;
        } else {
            split(t->l, l, t->l, k, add), r = t;
        }

        update_size(t);
        operation(t);
    }

    pnode root;
    implicit_treap() { root = nullptr; }

    void insert(int pos, int val) {
        if(!root) {
            pnode nw = new node(val);
            root = nw;
            return;
        }

        pnode l, r, nw = new node(val);

        split(root, l, r, pos - 1);
        merge(l, l, nw);
        merge(root, l, r);
    }

    int query_max(int qL, int qR) {
        pnode l, r, mid;

        split(root, l, r, qL - 1);
        split(r, mid, r, qR - qL);

        int ret = mid ? mid->mx : -1;

        merge(r, mid, r);
        merge(root, l, r);

        return ret;
    }
};
