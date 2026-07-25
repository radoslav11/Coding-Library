#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include <bits/stdc++.h>
using namespace std;

template<class T = string>
class SuffixArray {
  private:
    using G = conditional_t<
        is_same_v<T, const char*> || is_same_v<T, char*> || is_array_v<T>, char,
        typename T::value_type>;

    void build_sa(const T& s) {
        sa.resize(n);
        rnk.resize(n);
        vector<int> tmp(n);

        for(int i = 0; i < n; i++) {
            sa[i] = i;
            rnk[i] = (int)s[i];
        }

        for(int k = 1; k < n; k <<= 1) {
            auto cmp = [&](int x, int y) {
                if(rnk[x] != rnk[y]) {
                    return rnk[x] < rnk[y];
                }

                int rx = x + k < n ? rnk[x + k] : -1;
                int ry = y + k < n ? rnk[y + k] : -1;
                return rx < ry;
            };

            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for(int i = 1; i < n; i++) {
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            }

            rnk = tmp;
            if(rnk[sa[n - 1]] == n - 1) {
                break;
            }
        }
    }

    void build_lcp(const T& s) {
        lcp.assign(n, 0);
        int h = 0;
        for(int i = 0; i < n; i++) {
            if(rnk[i] > 0) {
                int j = sa[rnk[i] - 1];
                while(i + h < n && j + h < n && s[i + h] == s[j + h]) {
                    h++;
                }

                lcp[rnk[i]] = h;
                if(h > 0) {
                    h--;
                }
            } else {
                h = 0;
            }
        }
    }

  public:
    int n;
    vector<int> sa, rnk, lcp;

    SuffixArray() : n(0) {}
    SuffixArray(const T& s) { init(s); }

    void clear() {
        n = 0;
        sa.clear();
        rnk.clear();
        lcp.clear();
    }

    void init(const T& s) {
        clear();
        n = (int)size(s);
        if(n == 0) {
            return;
        }

        build_sa(s);
        build_lcp(s);
    }
};

// lcp[] is only the LCP of ADJACENT suffixes in sorted order. For the LCP of
// two arbitrary suffixes, RMQ over lcp between their ranks (pair with
// data_structures/sparse_table.hpp). This also gives O(1) lexicographic
// comparison of any two substrings.
//
//   int min_int(int a, int b) { return min(a, b); }
//
//   SuffixArray<string> sa(s);
//   SparseTable<int, min_int> rmq;
//   rmq.init(sa.lcp);                  // query is inclusive; lcp[0] never hit
//
//   auto lcp_suffix = [&](int i, int j) {        // LCP of suffixes i and j (i != j)
//       int ri = sa.rnk[i], rj = sa.rnk[j];
//       if(ri > rj) { swap(ri, rj); }
//       return rmq.query(ri + 1, rj);            // min lcp over (ri, rj]
//   };
//
//   auto greater_sub = [&](int i1, int e1, int i2, int e2) {  // s[i1..e1) > s[i2..e2)?
//       int len1 = e1 - i1, len2 = e2 - i2;
//       int common = lcp_suffix(i1, i2);
//       if(common >= min(len1, len2)) { return len1 > len2; } // prefix -> longer wins
//       return sa.rnk[i1] > sa.rnk[i2];                       // else rank decides
//   };

#endif  // SUFFIX_ARRAY_HPP
