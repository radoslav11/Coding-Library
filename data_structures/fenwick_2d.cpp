#include <bits/stdc++.h>
using namespace std;

template<class T, T (*merge)(T, T), T (*identity)()>
struct fenwick_2d {
    int sz;
    vector<unordered_map<int, T>> t;

    void init(int _sz) {
        sz = _sz;
        t.assign(sz + 1, unordered_map<int, T>());
    }

    void update_row(int row, int idx, T val) {
        if(idx == 0) {
            return;
        }
        for(; idx <= sz; idx += (idx & -idx)) {
            if(t[row].find(idx) == t[row].end()) {
                t[row][idx] = identity();
            } else {
                t[row][idx] = merge(t[row][idx], val);
            }
        }
    }

    T query_row(int row, int idx) {
        T ans = identity();
        for(; idx >= 1; idx -= (idx & -idx)) {
            if(t[row].find(idx) == t[row].end()) {
                ans = merge(identity(), ans);
            } else {
                ans = merge(t[row][idx], ans);
            }
        }

        return ans;
    }

    void update(int row, int col, T val) {
        if(col == 0) {
            return;
        }
        for(; row <= sz; row += (row & -row)) {
            update_row(row, col, val);
        }
    }

    T query(int row, int col) {
        T ans = identity();
        for(; row >= 1; row -= (row & -row)) {
            ans = merge(query_row(row, col), ans);
        }

        return ans;
    }
};

int min_custom(int a, int b) { return min(a, b); }
int max_custom(int a, int b) { return max(a, b); }
int sum_custom(int a, int b) { return a + b; }

int identity_min() { return INT_MAX; }
int identity_max() { return INT_MIN; }
int identity_sum() { return 0; }

fenwick_2d<int, sum_custom, identity_sum> ft;
