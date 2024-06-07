#include <bits/stdc++.h>
#include "coding_library/data_structures/fenwick_range_update.hpp"
#include "coding_library/tree/lca.hpp"

using namespace std;

class vertex_add_path_sum : lca_utils {
  private:
    fenwick_range_update<int> ft;

  public:
    vertex_add_path_sum() {}

    void prepare(int root = 1) {
        lca_utils::prepare(root);
        ft.init(dfs_time);
    }

    int query(int u, int v) {
        int l = lca(u, v);
        int ans =
            ft.query(in_time[u]) + ft.query(in_time[v]) - ft.query(in_time[l]);
        if(par_up[l][0] != l) {
            ans -= ft.query(in_time[par_up[l][0]]);
        }

        return ans;
    }

    void update(int u, int x) { ft.update(in_time[u], out_time[u], x); }
};
