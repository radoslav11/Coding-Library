#include <bits/stdc++.h>
using namespace std;

template<class T>
class Fenwick {
  private:
    int sz, log_size;
    vector<T> tr;

  public:
    void init(int n) {
        sz = n + 1;
        log_size = 31 - __builtin_clz(sz);
        tr.assign(sz + 1, 0);
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

    T query(int idx) {
        T ans = 0;
        for(; idx >= 1; idx -= (idx & -idx)) {
            ans += tr[idx];
        }

        return ans;
    }

    T query(int l, int r) { return query(r) - query(l - 1); }

    int find_kth(T k) {
        int idx = 0;
        for(int i = log_size; i >= 0; i--) {
            if(idx + (1 << i) < sz && tr[idx + (1 << i)] < k) {
                k -= tr[idx + (1 << i)];
                idx += (1 << i);
            }
        }
        return idx + 1;
    }
};
