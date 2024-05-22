#include <bits/stdc++.h>
using namespace std;

template<class T>
class fenwick {
  private:
    int sz;
    vector<T> tr;

  public:
    void init(int n) {
        sz = n + 1;
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
};
