#ifndef REROOT_DP_HPP
#define REROOT_DP_HPP

#include <bits/stdc++.h>
using namespace std;

// Generic rerooting DP. For every vertex v it computes the value of the tree
// rooted at v, in O(n) total. Sibling exclusion uses prefix/suffix merges, so
// merge only has to be a commutative monoid (no inverse needed). All scratch is
// reused across the recursion, so a call allocates nothing per node.
//
// The DP policy describes the DP through two types and four operations:
//   using Agg;                          aggregate of several child contributions
//   using Val;                          value a subtree sends up to its parent
//   static Agg e();                     empty aggregate (no children)
//   static Agg single(Val x);           lift one child's contribution
//   static Agg merge(Agg a, Agg b);     combine two aggregates (monoid)
//   static Val finalize(Agg a, int v);  subtree value of v from its children
//
// down[v]   = finalize(merge of single(down[c]) over children c, v).
// answer[v] = finalize(merge of single(contribution) over ALL neighbours, v),
//             the value of the whole tree when rooted at v.
//
// Reuse: call init(n) once, then for each tree clear adj[1..k], re-add its
// edges, solve(), and read answer[1..k]. Recursive; raise the stack if deep.

template<class DP>
class RerootDP {
  public:
    using Agg = typename DP::Agg;
    using Val = typename DP::Val;

    int n;
    vector<vector<int>> adj;
    vector<Val> down, up, answer;

    RerootDP(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n + 1, {});
        down.assign(n + 1, Val());
        up.assign(n + 1, Val());
        answer.assign(n + 1, Val());
        cont.assign(n + 1, Val());
        pref.assign(n + 2, DP::e());
        suff.assign(n + 2, DP::e());
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void solve(int root = 1) {
        pre_dfs(root, 0);
        reroot_dfs(root, 0);
    }

  private:
    vector<Val> cont;
    vector<Agg> pref, suff;

    void pre_dfs(int v, int p) {
        Agg acc = DP::e();
        for(int u: adj[v]) {
            if(u != p) {
                pre_dfs(u, v);
                acc = DP::merge(acc, DP::single(down[u]));
            }
        }

        down[v] = DP::finalize(acc, v);
    }

    void reroot_dfs(int v, int p) {
        int deg = (int)adj[v].size();
        for(int j = 0; j < deg; j++) {
            int u = adj[v][j];
            cont[j] = u == p ? up[v] : down[u];
        }

        pref[0] = DP::e();
        suff[deg] = DP::e();
        for(int j = 0; j < deg; j++) {
            pref[j + 1] = DP::merge(pref[j], DP::single(cont[j]));
        }
        for(int j = deg - 1; j >= 0; j--) {
            suff[j] = DP::merge(DP::single(cont[j]), suff[j + 1]);
        }

        answer[v] = DP::finalize(pref[deg], v);
        for(int j = 0; j < deg; j++) {
            int u = adj[v][j];
            if(u != p) {
                up[u] = DP::finalize(DP::merge(pref[j], suff[j + 1]), v);
            }
        }

        for(int j = 0; j < deg; j++) {
            int u = adj[v][j];
            if(u != p) {
                reroot_dfs(u, v);
            }
        }
    }
};

// Example: max over all roots of (1 + second largest neighbour value), the
// static version of the rerooting in this folder's "Game on Growing Tree".
//
// struct SecondMaxDP {
//     using Val = int;             // 1 + second largest among the other sides
//     using Agg = pair<int, int>;  // (largest, second largest), kept ordered
//     static Agg e() { return {0, 0}; }
//     static Agg single(Val x) { return {x, 0}; }
//     static Agg merge(Agg a, Agg b) {
//         if(a.first < b.first) {
//             swap(a, b);
//         }
//         return {a.first, max(a.second, b.first)};
//     }
//     static Val finalize(Agg a, int) { return 1 + a.second; }
// };
//
// RerootDP<SecondMaxDP> dp(n);
// for(auto [u, v]: edges) dp.add_edge(u, v);
// dp.solve();
// int score = *max_element(dp.answer.begin() + 1, dp.answer.end());

#endif  // REROOT_DP_HPP
