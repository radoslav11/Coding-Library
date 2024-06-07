#include <bits/stdc++.h>
#include "coding_library/data_structures/fenwick_range_update.hpp"
using namespace std;

template<class T>
class fenwick_ru_rq {
    int sz;
    fenwick_range_update<T> t0, t1;

    void init(int _sz) {
        sz = _sz;
        t0.init(sz + 1);
        t1.init(sz + 1);
    }

    void update(int l, int r, T val) {
        t0.update(l, r, val);
        t1.update(l, r, -(l - 1) * val);
        t1.update(r + 1, sz, (r - l + 1) * val);
    }

    T query(int idx) { return t0.query(idx) * (T)idx + t1.query(idx); }
    T query(int l, int r) { return query(r) - query(l - 1); }
};
