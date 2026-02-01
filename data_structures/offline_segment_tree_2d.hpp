#include <bits/stdc++.h>
#include <coding_library/data_structures/fenwick.hpp>

using namespace std;

template<class T>
class OfflineSegmentTree2d {
  private:
    int n;
    vector<vector<int>> ys;
    vector<Fenwick<T>> ft;

    void collect(int v, int tl, int tr, int pos, int y) {
        ys[v].push_back(y);
        if(tl == tr) {
            return;
        }
        int tm = (tl + tr) / 2;
        if(pos <= tm) {
            collect(v * 2, tl, tm, pos, y);
        } else {
            collect(v * 2 + 1, tm + 1, tr, pos, y);
        }
    }

    void build_compression(int v) {
        sort(ys[v].begin(), ys[v].end());
        ys[v].erase(unique(ys[v].begin(), ys[v].end()), ys[v].end());
        ft[v].init(ys[v].size() + 2);
    }

    void update_rec(int v, int tl, int tr, int pos, int y, T val) {
        if(ys[v].empty()) {
            return;
        }
        auto it = lower_bound(ys[v].begin(), ys[v].end(), y);
        if(it == ys[v].end() || *it != y) {
            return;
        }
        int rank = (it - ys[v].begin()) + 1;
        ft[v].update(rank, val);
        if(tl == tr) {
            return;
        }
        int tm = (tl + tr) / 2;
        if(pos <= tm) {
            update_rec(v * 2, tl, tm, pos, y, val);
        } else {
            update_rec(v * 2 + 1, tm + 1, tr, pos, y, val);
        }
    }

    T query_rec(int v, int tl, int tr, int l, int r, int yl, int yr) {
        if(l > tr || r < tl) {
            return 0;
        }
        if(l <= tl && tr <= r) {
            if(ys[v].empty()) {
                return 0;
            }
            auto itl = lower_bound(ys[v].begin(), ys[v].end(), yl);
            auto itr = upper_bound(itl, ys[v].end(), yr);
            int ly = (itl - ys[v].begin()) + 1;
            int ry = (itr - ys[v].begin());
            return ft[v].query(ly, ry);
        }
        int tm = (tl + tr) / 2;
        return query_rec(v * 2, tl, tm, l, r, yl, yr) +
               query_rec(v * 2 + 1, tm + 1, tr, l, r, yl, yr);
    }

  public:
    void init(int _n) {
        n = _n;
        int tsz = 4 * (n + 10);
        ys.assign(tsz, {});
        ft.resize(tsz);
    }

    void prepare_update(int x, int y) { collect(1, 1, n, x, y); }

    void prepare() {
        for(int v = 1; v < 4 * (n + 10); ++v) {
            if(!ys[v].empty()) {
                build_compression(v);
            }
        }
    }

    void update(int x, int y, T val) { update_rec(1, 1, n, x, y, val); }

    T query(int xl, int xr, int yl, int yr) {
        return query_rec(1, 1, n, xl, xr, yl, yr);
    }
};
