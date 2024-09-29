#include <bits/stdc++.h>
#include <coding_library/graph/dsu.hpp>
using namespace std;

/*
The structures resembles a set on N positions that are said to initially be
active, and supports the following updates:
- Remove some active position from the set.
- query_left(i) to find the first active position before i (inclusive). 
- query_right(i) to find the first active position after i (inclusive).

Main idea is to do DSU on blocks of size w=64 working in O(N alpha(N) / w) =
O(N) overall, and then answer small queries in O(1) using bitwise operations.

Based on Nachia's implementation:
https://codeforces.com/contest/2018/submission/283309295
*/

class DecrementalConstantSet {
  private:
    const uint64_t FULL_MASK = ~0ull;

    int n, b;
    vector<uint64_t> mask;
    vector<int> large_block_left, large_block_right;
    DSU dsu;

    int small_query_prefix(int c, uint64_t filter_mask) {
        uint64_t z = mask[c] & filter_mask;
        return z ? (c << 6) + __builtin_ctzll(z) : -1;
    }

    int small_query_suffix(int c, uint64_t filter_mask) {
        uint64_t z = mask[c] & filter_mask;
        return z ? (c << 6) + 63 - __builtin_clzll(z) : -1;
    }

  public:
    DecrementalConstantSet(int _n)
        : n(_n + 2),
          b((n + 64) >> 6),
          mask(b, FULL_MASK),
          large_block_left(b + 1),
          large_block_right(b + 1),
          dsu(b) {
        iota(large_block_left.begin(), large_block_left.end(), 0);
        iota(large_block_right.begin(), large_block_right.end(), 0);
    }

    void remove(int _i) {
        // Indices are +1
        int i = _i + 1;
        int c = i >> 6, d = i & 63;
        mask[c] &= ~(1ull << d);
        if(mask[c] == 0) {
            for(int dir: {-1, 1}) {
                if(c + dir < 0 || c + dir >= b || mask[c + dir]) {
                    continue;
                }
                int root = dsu.root(c);
                int next_root = dsu.root(c + dir);
                if(root != next_root) {
                    int new_root = dsu.unite(root, next_root);
                    large_block_left[new_root] =
                        min(large_block_left[root],
                            large_block_left[next_root]);
                    large_block_right[new_root] =
                        max(large_block_right[root],
                            large_block_right[next_root]);
                }
            }
        }
    }

    int query_right(int _i) {
        if(_i >= n) {
            return n;
        }
        // Indices are +1 and we undo when we return
        int i = max(_i, 0) + 1;
        int c = i >> 6, d = i & 63;
        if(d > 0) {
            int x = small_query_prefix(c, FULL_MASK << d);
            if(x >= 0) {
                return x - 1;
            }
            c++;
        }

        if(!mask[c]) {
            c = large_block_right[dsu.root(c)] + 1;
        }
        return small_query_prefix(c, FULL_MASK) - 1;
    }

    int query_left(int _i) {
        if(_i < 0) {
            return -1;
        }
        // Indices are +1 and we undo when we return
        int i = min(_i + 1, n);
        int c = i >> 6, d = i & 63;

        if(d < 63) {
            int x = small_query_suffix(c, (1ull << (d + 1)) - 1);
            if(x >= 0) {
                return x - 1;
            }
            c--;
        }

        if(!mask[c]) {
            c = large_block_left[dsu.root(c)] - 1;
        }
        return small_query_suffix(c, FULL_MASK) - 1;
    }
};
