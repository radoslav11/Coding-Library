#include <bits/stdc++.h>
using namespace std;

// Primal simplex for:
//
//     maximize   c^T x
//     subject to A x <= b,  x >= 0
//
// constraints is m x n. Status::OPTIMAL returns the objective value in `value`
// and, if requested, one optimal vertex in `x`.
//
// feasible() checks nonemptiness of { x >= 0 : A x <= b } by enumerating LP
// vertices (fast when n is small, e.g. n <= 60).

template<class T = long double>
class Simplex {
  public:
    enum class Status { OPTIMAL, INFEASIBLE, UNBOUNDED };

    struct Result {
        Status status = Status::INFEASIBLE;
        T value = 0;
        vector<T> x;
    };

    static T eps() { return T(1e-9); }

    static T inf() {
        if constexpr(is_floating_point_v<T>) {
            return numeric_limits<T>::infinity();
        }
        return numeric_limits<T>::max() / T(4);
    }

    Result solve(
        const vector<vector<T>>& constraints, const vector<T>& rhs,
        const vector<T>& objective, bool recover_x = false
    ) {
        Result res;
        m = (int)rhs.size();
        n = (int)objective.size();
        if(m == 0) {
            res.status = Status::OPTIMAL;
            if(recover_x) {
                res.x.assign(n, 0);
            }
            return res;
        }

        const int slack = n + m;
        const int rhs_col = n + m + 1;
        const int width = n + m + 2;

        N.assign(width - 1, -1);
        B.assign(m + 1, 0);
        D.assign(m + 2, vector<T>(width, 0));

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                D[i][j] = constraints[i][j];
            }
            D[i][n + i] = 1;
            D[i][rhs_col] = rhs[i];
            B[i] = n + i;
        }
        for(int j = 0; j < slack; j++) {
            N[j] = j;
        }
        for(int j = 0; j < n; j++) {
            D[m][j] = -objective[j];
        }
        N[slack] = -1;
        D[m + 1] = D[m];
        D[m + 1][slack] = 1;
        B[m] = m + 1;

        int x = n;
        for(int i = 0; i < m; i++) {
            if(D[i][rhs_col] < -eps()) {
                x = slack;
            }
        }

        if(!run_phase(1, x, slack, rhs_col)) {
            return res;
        }
        if(!run_phase(2, n, slack, rhs_col)) {
            res.status = Status::UNBOUNDED;
            return res;
        }

        res.status = Status::OPTIMAL;
        res.value = D[m][rhs_col];
        if(recover_x) {
            res.x.assign(n, 0);
            for(int i = 0; i < m; i++) {
                if(B[i] < n) {
                    res.x[B[i]] = D[i][rhs_col];
                }
            }
        }
        return res;
    }

    bool feasible(const vector<vector<T>>& constraints, const vector<T>& rhs) {
        return feasible_vertices(constraints, rhs);
    }

    static bool feasible_vertices(
        const vector<vector<T>>& rows, const vector<T>& rhs
    ) {
        if(rows.empty()) {
            return true;
        }
        int m = (int)rows[0].size();
        int r = (int)rows.size();
        if(m == 0) {
            return true;
        }

        auto check_point = [&](const vector<T>& x) {
            for(T t: x) {
                if(t < -eps()) {
                    return false;
                }
            }
            for(int i = 0; i < r; i++) {
                T s = 0;
                for(int j = 0; j < m; j++) {
                    s += rows[i][j] * x[j];
                }
                if(s > rhs[i] + eps()) {
                    return false;
                }
            }
            return true;
        };

        vector<int> comb(m);
        iota(comb.begin(), comb.end(), 0);
        while(true) {
            vector<vector<T>> a(m, vector<T>(m));
            vector<T> b(m);
            for(int i = 0; i < m; i++) {
                a[i] = rows[comb[i]];
                b[i] = rhs[comb[i]];
            }
            vector<T> x;
            if(gauss_solve(a, b, x) && check_point(x)) {
                return true;
            }
            int i = m - 1;
            while(i >= 0 && comb[i] == i + r - m) {
                i--;
            }
            if(i < 0) {
                break;
            }
            comb[i]++;
            for(int j = i + 1; j < m; j++) {
                comb[j] = comb[j - 1] + 1;
            }
        }
        return false;
    }

  private:
    int m, n;
    vector<int> N, B;
    vector<vector<T>> D;

    static bool gauss_solve(vector<vector<T>> a, vector<T> b, vector<T>& x) {
        int rows = (int)a.size();
        int cols = (int)a[0].size();
        x.assign(cols, 0);
        vector<int> where(cols, -1);
        for(int col = 0, row = 0; col < cols && row < rows; col++) {
            int sel = row;
            for(int i = row; i < rows; i++) {
                if(fabs(a[i][col]) > fabs(a[sel][col])) {
                    sel = i;
                }
            }
            if(fabs(a[sel][col]) < eps()) {
                continue;
            }
            swap(a[sel], a[row]);
            swap(b[sel], b[row]);
            T div = a[row][col];
            for(int j = col; j < cols; j++) {
                a[row][j] /= div;
            }
            b[row] /= div;
            for(int i = 0; i < rows; i++) {
                if(i == row) {
                    continue;
                }
                T f = a[i][col];
                for(int j = col; j < cols; j++) {
                    a[i][j] -= f * a[row][j];
                }
                b[i] -= f * b[row];
            }
            where[col] = row++;
        }
        for(int j = 0; j < cols; j++) {
            if(where[j] != -1) {
                x[j] = b[where[j]];
            }
        }
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                if(where[j] == -1 && fabs(a[i][j]) > eps() &&
                   fabs(b[i]) > eps()) {
                    return false;
                }
            }
        }
        return true;
    }

    void pivot(int r, int s, int width) {
        T pv = D[r][s];
        for(int j = 0; j < width; j++) {
            D[r][j] /= pv;
        }
        for(int i = 0; i < m + 2; i++) {
            if(i == r) {
                continue;
            }
            if(fabs(D[i][s]) <= eps()) {
                continue;
            }
            T f = D[i][s];
            for(int j = 0; j < width; j++) {
                D[i][j] -= f * D[r][j];
            }
        }
        B[r] = N[s];
        N[s] = -1;
    }

    bool run_phase(int phase, int x, int slack, int rhs_col) {
        int width = n + m + 2;
        int obj = phase == 1 ? m + 1 : m;
        for(int iter = 0; iter < 10000; iter++) {
            int s = -1;
            if(phase == 1) {
                for(int j = 0; j < slack; j++) {
                    if(N[j] == -1 && j != x && D[obj][j] > eps()) {
                        s = j;
                    }
                }
            } else {
                T best = -eps();
                for(int j = 0; j < slack; j++) {
                    if(D[obj][j] < best) {
                        best = D[obj][j];
                        s = j;
                    }
                }
            }
            if(s == -1) {
                if(phase == 1) {
                    for(int i = 0; i < m; i++) {
                        if(B[i] == slack) {
                            return false;
                        }
                    }
                }
                return true;
            }
            int r = -1;
            T best_ratio = inf();
            for(int i = 0; i < m; i++) {
                if(phase == 1) {
                    if(D[i][s] < -eps()) {
                        T df = -D[i][rhs_col] / D[i][s];
                        if(r == -1 || df < -D[r][rhs_col] / D[r][s] - eps()) {
                            r = i;
                        }
                    }
                } else if(D[i][s] > eps()) {
                    T ratio = D[i][rhs_col] / D[i][s];
                    if(ratio < best_ratio - eps()) {
                        best_ratio = ratio;
                        r = i;
                    }
                }
            }
            if(r == -1) {
                return false;
            }
            pivot(r, s, width);
        }
        return false;
    }
};
