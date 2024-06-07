#include <bits/stdc++.h>
using namespace std;

template<class T>
struct fenwick {
    int sz;
    vector<T> tr;

    void init(int n) {
        sz = n + 1;
        tr.assign(sz + 1, 0);
    }

    T query(int idx) {
        T ans = 0;
        for(; idx >= 1; idx -= (idx & -idx)) {
            ans += tr[idx];
        }

        return ans;
    }

    void update(int idx, T val) {
        if(idx <= 0) {
            assert(false);
            return;
        }
        for(; idx <= sz; idx += (idx & -idx)) {
            tr[idx] += val;
        }
    }

    T query(int l, int r) { return query(r) - query(l - 1); }
};

template<class T>
struct compressed_fenwick {
    fenwick<T> t;
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
