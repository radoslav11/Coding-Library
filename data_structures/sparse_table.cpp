#include <bits/stdc++.h>
#define endl '\n'

using namespace std;
template<class T, class T2>
inline void chkmax(T &x, const T2 &y) {
    if(x < y) {
        x = y;
    }
}
template<class T, class T2>
inline void chkmin(T &x, const T2 &y) {
    if(x > y) {
        x = y;
    }
}

template<class T, T (*merge)(T, T)>
class sparse_table {
  private:
    int n;
    vector<vector<T>> dp;
    vector<int> prec_lg2;

  public:
    sparse_table() {
        n = 0;
        dp.clear();
        prec_lg2.clear();
    }

    void init(const vector<T> &a) {
        n = a.size();
        prec_lg2.resize(n + 1);
        for(int i = 2; i <= n; i++) {
            prec_lg2[i] = prec_lg2[i >> 1] + 1;
        }

        dp.assign(prec_lg2[n] + 1, vector<T>(n));
        dp[0] = a;

        for(int j = 1; (1 << j) <= n; j++) {
            for(int i = 0; i + (1 << j) < n; i++) {
                dp[j][i] = merge(dp[j - 1][i], dp[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int l, int r) {
        int k = prec_lg2[r - l + 1];
        return merge(dp[k][l], dp[k][r - (1 << k) + 1]);
    }
};

int min_custom(int a, int b) { return min(a, b); }
sparse_table<int, min_custom> st;

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
