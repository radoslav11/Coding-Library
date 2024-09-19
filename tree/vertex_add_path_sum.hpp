#include <bits/stdc++.h>
#include "coding_library/data_structures/fenwick_range_update.hpp"
#include "coding_library/tree/lca.hpp"

using namespace std;

class VertexAddPathSum : LCAUtils {
  private:
    FenwickRangeUpdate<int> ft;

  public:
    VertexAddPathSum() {}

    void prepare(int root = 1) {
        LCAUtils::prepare(root);
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
