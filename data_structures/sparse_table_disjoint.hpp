#include <bits/stdc++.h>
using namespace std;

template<class T, T (*merge)(T, T)>
class SparseTableDisjoint {
  private:
    int n;
    vector<int> prec_lg2, mask;
    vector<vector<T>> dp;
    vector<T> a;

    void init_dnc(int l, int r, int lvl) {
        if(l == r) {
            dp[lvl][l] = a[l];
            return;
        }

        int m = (l + r) >> 1;
        init_dnc(l, m, lvl + 1);
        init_dnc(m + 1, r, lvl + 1);

        dp[lvl][m] = a[m];
        for(int i = m - 1; i >= l; i--) {
            dp[lvl][i] = merge(a[i], dp[lvl][i + 1]);
        }

        dp[lvl][m + 1] = a[m + 1];
        mask[m + 1] |= 1 << lvl;
        for(int i = m + 2; i <= r; i++) {
            dp[lvl][i] = merge(dp[lvl][i - 1], a[i]);
            mask[i] |= 1 << lvl;
        }
    }

  public:
    SparseTableDisjoint() {
        n = 0;
        a.clear();
        dp.clear();
        prec_lg2.clear();
        mask.clear();
    }

    void init(const vector<T>& _a) {
        a = _a;
        n = a.size();
        prec_lg2.assign(4 * n + 1, 0);
        for(int i = 2; i <= 4 * n; i++) {
            prec_lg2[i] = prec_lg2[i >> 1] + 1;
        }

        dp.assign(prec_lg2[n] + 2, vector<T>(n));
        mask.assign(n, 0);
        init_dnc(0, n - 1, 0);
    }

    T query(int l, int r) {
        if(l == r) {
            return a[l];
        }

        int mask_diff = mask[l] ^ mask[r];
        int k = prec_lg2[mask_diff & -mask_diff];
        return merge(dp[k][l], dp[k][r]);
    }
};

// int min_custom(int a, int b) { return min(a, b); }
// sparse_table_disjoint<int, min_custom> st;
