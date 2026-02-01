#include <bits/stdc++.h>
using namespace std;

// InnerOrderedSet should implement:
//     1) insert(x), which adds one copy of x.
//     2) erase(x), which removes one copy of x.
//     3) count_leq(x), which gives the number of elements <= x.
//
// Example with a treap is given below.

template<class InnerOrderedSet>
class MergeSortTree {
  private:
    int n;
    vector<InnerOrderedSet> nodes;

    void update_rec(int v, int tl, int tr, int pos, int y, int delta) {
        if(delta == 1) {
            nodes[v].insert(y, 1);
        } else if(delta == -1) {
            nodes[v].erase(y);
        }

        if(tl == tr) {
            return;
        }
        int tm = (tl + tr) / 2;
        if(pos <= tm) {
            update_rec(v * 2, tl, tm, pos, y, delta);
        } else {
            update_rec(v * 2 + 1, tm + 1, tr, pos, y, delta);
        }
    }

    int query_rec(int v, int tl, int tr, int ql, int qr, int k) {
        if(ql > tr || qr < tl) {
            return 0;
        }
        if(ql <= tl && tr <= qr) {
            return nodes[v].count_leq(k);
        }
        int tm = (tl + tr) / 2;
        return query_rec(v * 2, tl, tm, ql, qr, k) +
               query_rec(v * 2 + 1, tm + 1, tr, ql, qr, k);
    }

  public:
    void init(int _n) {
        n = _n;
        nodes.resize(4 * (n + 10));
    }

    void update(int x, int y, int delta) { update_rec(1, 1, n, x, y, delta); }

    int query(int qxl, int qxr, int k) {
        return query_rec(1, 1, n, qxl, qxr, k);
    }

    int query(int qxl, int qxr, int qyl, int qyr) {
        return query_rec(1, 1, n, qxl, qxr, qyr) -
               query_rec(1, 1, n, qxl, qxr, qyl - 1);
    }
};

// #include <coding_library/data_structures/treap.hpp>
// int sum_merge(int a, int b) { return a + b; }
// using CountTreap = Treap<int, int, sum_merge>;
// MergeSortTree<CountTreap> st;
