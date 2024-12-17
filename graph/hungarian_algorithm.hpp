#include <bits/stdc++.h>
using namespace std;

// Based on http://e-maxx.ru/algo/assignment_hungary#6 but wrapped in a class
// and zero-based indexing. Optimizes the assignment so that the cost is
// minimized. Also make sure that n <= m.

template<class T>
class HungarianAlgorithm {
  private:
    const T INF = numeric_limits<T>::max() / 2;
    vector<int> way;

  public:
    int n, m;
    vector<vector<T>> cost;
    vector<int> assignment;
    vector<T> pot_left, pot_right;

    HungarianAlgorithm(const vector<vector<T>>& a) {
        n = a.size();
        m = a[0].size();
        assert(n <= m);

        cost.assign(n + 1, vector<T>(m + 1));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cost[i][j] = a[i][j];
            }
        }

        pot_left.assign(n + 1, 0);
        pot_right.assign(m + 1, 0);
        assignment.assign(m + 1, n);
        way.assign(m + 1, m);

        for(int i = 0; i < n; i++) {
            assignment[m] = i;
            int j0 = m;
            vector<T> minv(m + 1, INF);
            vector<bool> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = assignment[j0], j1 = m;
                T delta = INF;
                for(int j = 0; j < m; j++) {
                    if(!used[j]) {
                        T cur = cost[i0][j] - pot_left[i0] - pot_right[j];
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
                        pot_left[assignment[j]] += delta;
                        pot_right[j] -= delta;
                    } else {
                        minv[j] -= delta;
                    }
                }
                j0 = j1;
            } while(assignment[j0] != n);

            do {
                int j1 = way[j0];
                assignment[j0] = assignment[j1];
                j0 = j1;
            } while(j0 != m);
        }
    }

    T get_cost() {
        T ans = 0;
        for(int j = 0; j < m; j++) {
            ans += cost[assignment[j]][j];
        }
        return ans;
    }
};
