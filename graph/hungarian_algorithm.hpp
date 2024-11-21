#include <bits/stdc++.h>
using namespace std;

// Based on http://e-maxx.ru/algo/assignment_hungary#6 but wrapped in a class
// and zero-based indexing.

template<class T>
class HungarianAlgorithm {
  private:
    const T INF = numeric_limits<T>::max() / 2;
    vector<vector<T>> cost;

  public:
    vector<int> assignment;

    HungarianAlgorithm(const vector<vector<T>>& a) {
        int n = a.size(), m = a[0].size();
        cost.assign(n + 1, vector<T>(m + 1));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cost[i][j] = a[i][j];
            }
        }

        vector<T> u(n + 1), v(m + 1);
        vector<int> p(m + 1, n), way(m + 1, n);
        for(int i = 0; i < n; i++) {
            p[m] = i;
            int j0 = m;
            vector<T> minv(m + 1, INF);
            vector<bool> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], j1;
                T delta = INF;
                for(int j = 0; j < m; j++) {
                    if(!used[j]) {
                        T cur = cost[i0][j] - u[i0] - v[j];
                        if(cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = j0;
                        }
                        if(minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                }
                for(int j = 0; j <= m; j++) {
                    if(used[j]) {
                        u[p[j]] += delta;
                        v[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while(p[j0] != n);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while(j0 != m);
        }

        assignment = vector<int>(begin(p), end(p) - 1);
    }

    T get_cost() {
        T ans = 0;
        for(int i = 0; i < (int)assignment.size(); i++) {
            ans += cost[assignment[i]][i];
        }
        return ans;
    }
};
