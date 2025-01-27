#include <bits/stdc++.h>
using namespace std;

/* Based on https://codeforces.com/contest/2063/problem/F2. Main idea is to
   do reverse small-to-large trick and we can store everything with arrays
   instead of using a complicated approach with sets and maps.

   This version only counts the "non-used" elements in the range, so
   we might want to redefine this in a different problem.
*/

class NonintersectingRangeTree {
  public:
    vector<int> pnt, size, par, par_idx;
    vector<bool> used;

    NonintersectingRangeTree(int n) {
        pnt.assign(n + 2, 0);
        iota(pnt.begin(), pnt.end(), 1);

        par_idx.assign(n + 2, 0);
        par = {0};

        size.assign(n + 2, 0);
        used.assign(n + 2, false);

        used[0] = true;
        pnt[0] = n + 2;
        pnt[n + 1] = n + 1;
        size[0] = n;
    }

    void add(int l, int r) {
        pnt[l] = r + 1;
        pnt[r] = r;
        used[l] = true;

        int p = get_parent(l);
        int out_ptr = p + 1, ins_ptr = l + 1;
        int out_size = 0, ins_size = 0;

        while(pnt[out_ptr] != out_ptr && pnt[ins_ptr] != ins_ptr) {
            out_size += !used[out_ptr];
            ins_size += !used[ins_ptr];
            out_ptr = pnt[out_ptr];
            ins_ptr = pnt[ins_ptr];
        }

        int upd_ptr, new_par_idx = par.size();
        if(pnt[out_ptr] == out_ptr) {
            upd_ptr = p + 1;
            par.push_back(p);
            par[par_idx[l]] = l;

            ins_size = size[p] - 2 - out_size;
            size[p] = out_size;
            size[l] = ins_size;
        } else {
            upd_ptr = l + 1;
            par.push_back(l);

            out_size = size[p] - 2 - ins_size;
            size[p] = out_size;
            size[l] = ins_size;
        }

        while(pnt[upd_ptr] != upd_ptr) {
            par_idx[upd_ptr] = new_par_idx;
            upd_ptr = pnt[upd_ptr];
        }
    }

    int get_parent(int x) { return par[par_idx[x]]; }
};
