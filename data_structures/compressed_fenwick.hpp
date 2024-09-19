#include <bits/stdc++.h>
#include <coding_library/data_structures/fenwick.hpp>
using namespace std;

template<class T>
struct CompressedFenwick {
    Fenwick<T> t;
    vector<int> li;

    int get_idx(int x) {
        return lower_bound(li.begin(), li.end(), x) - li.begin() + 1;
    }
    void update(int x, T val) { t.update(get_idx(x), val); }

    void add_to_prep(int x) { li.push_back(x); }
    void prepare() {
        sort(li.begin(), li.end());
        li.erase(unique(li.begin(), li.end()), li.end());
        t.init(li.size() + 2);
    }

    T query(int x) { return t.query(get_idx(x) - 1); }
    T size() { return t.query(t.sz); }
};
