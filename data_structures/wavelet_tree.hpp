#include <bits/stdc++.h>
using namespace std;

struct wavelet_tree {
    int lo, hi;
    wavelet_tree *l, *r;
    int *b, psz;

    wavelet_tree() {
        lo = 1;
        hi = 0;
        psz = 0;
        l = NULL;
        r = NULL;
    }

    void init(int *from, int *to, int x, int y) {
        lo = x, hi = y;
        if(lo == hi || from >= to) {
            return;
        }
        int mid = (lo + hi) >> 1;
        auto f = [mid](int x) { return x <= mid; };
        b = (int *)malloc((to - from + 2) * sizeof(int));
        psz = 0;
        b[psz++] = 0;
        for(auto it = from; it != to; it++) {
            b[psz] = (b[psz - 1] + f(*it)), psz++;
        }
        auto pivot = stable_partition(from, to, f);
        l = new wavelet_tree();
        l->init(from, pivot, lo, mid);
        r = new wavelet_tree();
        r->init(pivot, to, mid + 1, hi);
    }

    int kth(int l, int r, int k) {
        if(l > r) {
            return 0;
        }
        if(lo == hi) {
            return lo;
        }
        int inLeft = b[r] - b[l - 1], lb = b[l - 1], rb = b[r];
        if(k <= inLeft) {
            return this->l->kth(lb + 1, rb, k);
        }
        return this->r->kth(l - lb, r - rb, k - inLeft);
    }

    int LTE(int l, int r, int k) {
        if(l > r || k < lo) {
            return 0;
        }
        if(hi <= k) {
            return r - l + 1;
        }
        int lb = b[l - 1], rb = b[r];
        return this->l->LTE(lb + 1, rb, k) + this->r->LTE(l - lb, r - rb, k);
    }

    int count(int l, int r, int k) {
        if(l > r || k < lo || k > hi) {
            return 0;
        }
        if(lo == hi) {
            return r - l + 1;
        }
        int lb = b[l - 1], rb = b[r];
        int mid = (lo + hi) >> 1;
        if(k <= mid) {
            return this->l->count(lb + 1, rb, k);
        }
        return this->r->count(l - lb, r - rb, k);
    }

    ~wavelet_tree() {
        delete l;
        delete r;
    }
};
