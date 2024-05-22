#include <bits/stdc++.h>

using namespace std;

template<class T, class T1>
int chkmin(T &x, const T1 &y) {
    return x > y ? x = y, 1 : 0;
}

template<class T, class T1>
int chkmax(T &x, const T1 &y) {
    return x < y ? x = y, 1 : 0;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

template<class T, T (*merge)(T, T)>
class sparse_table_disjoint {
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
    sparse_table_disjoint() {
        n = 0;
        a.clear();
        dp.clear();
        prec_lg2.clear();
        mask.clear();
    }

    void init(const vector<T> &_a) {
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

int min_custom(int a, int b) { return min(a, b); }
sparse_table_disjoint<int, min_custom> st;

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
