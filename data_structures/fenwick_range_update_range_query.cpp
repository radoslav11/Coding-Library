#include <bits/stdc++.h>
using namespace std;

template<class T>
class fenwick_range_update {
  private:
    int sz;
    vector<T> tr;

    void update(int idx, T val) {
        if(idx <= 0) {
            assert(false);
            return;
        }
        for(; idx <= sz; idx += (idx & -idx)) {
            tr[idx] += val;
        }
    }

  public:
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

    void update(int l, int r, T val) {
        update(l, val);
        update(r + 1, -val);
    }
};

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
