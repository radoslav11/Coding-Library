#include <bits/stdc++.h>
using namespace std;

template<size_t N>
class GaussBitset {
  public:
    int n_var;
    vector<bitset<N + 1>> rows;
    vector<int> where;
    bool solvable = false;

    GaussBitset(int n_var_) : n_var(n_var_) {}

    void add_equation(const vector<int>& coeffs, int rhs) {
        bitset<N + 1> row;
        for(int i = 0; i < n_var; i++) {
            if(coeffs[i] % 2) {
                row[i] = 1;
            }
        }
        row[n_var] = rhs % 2;
        rows.push_back(row);
    }

    void eliminate() {
        int n_eq = (int)rows.size();
        where.assign(n_var, -1);
        int row = 0;
        for(int col = 0; col < n_var && row < n_eq; col++) {
            int sel = -1;
            for(int i = row; i < n_eq; i++) {
                if(rows[i][col]) {
                    sel = i;
                    break;
                }
            }
            if(sel == -1) {
                continue;
            }
            swap(rows[sel], rows[row]);
            where[col] = row;
            for(int i = 0; i < n_eq; i++) {
                if(i != row && rows[i][col]) {
                    rows[i] ^= rows[row];
                }
            }
            row++;
        }
        solvable = true;
        for(int i = row; i < n_eq; i++) {
            if(rows[i][n_var]) {
                solvable = false;
                break;
            }
        }
    }

    bool has_solution() const { return solvable; }

    vector<int> free_variables() const {
        vector<int> fv;
        for(int v = 0; v < n_var; v++) {
            if(where[v] == -1) {
                fv.push_back(v);
            }
        }
        return fv;
    }

    vector<int> any_solution() const {
        vector<int> x(n_var, 0);
        for(int v = 0; v < n_var; v++) {
            if(where[v] != -1) {
                x[v] = rows[where[v]][n_var];
            }
        }
        return x;
    }
};
