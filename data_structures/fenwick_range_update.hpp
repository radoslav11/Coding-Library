#include <bits/stdc++.h>
using namespace std;
const int MAXN = (1 << 20);

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
