// This is from https://codeforces.com/contest/1902/problem/F

#include <bits/stdc++.h>

using namespace std;

template<class T, class T1>
int chkmin(T &x, const T1 &y) {
    return x > y ? x = y, 1 : 0;
}

template<class T, class T1>
int chkmax(T &x, const T1 &y) {
    return x < y ? x = y, 1 : 0;
}

template<typename T1, typename T2>
ostream &operator<<(ostream &out, const pair<T1, T2> &x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream &operator>>(istream &in, pair<T1, T2> &x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream &operator>>(istream &in, vector<T> &a) {
    for(auto &x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream &operator<<(ostream &out, vector<T> &a) {
    for(auto &x: a) {
        out << x << ' ';
    }
    return out;
};

template<class T>
struct basis {
    int max_log;
    vector<T> base;

    void init(int _max_log) {
        max_log = _max_log;
        base.assign(max_log, 0);
    }

    void add(T val) {
        for(int i = max_log - 1; i >= 0; i--) {
            if((val >> i) & 1) {
                if(!base[i]) {
                    base[i] = val;
                    return;
                } else {
                    val ^= base[i];
                }
            }
        }
    }

    inline int size() {
        int sz = 0;
        for(int i = 0; i < max_log; i++) {
            sz += (bool)(base[i]);
        }
        return sz;
    }

    T max_xor() {
        T res = 0;
        for(int i = max_log - 1; i >= 0; i--) {
            if(!((res >> i) & 1) && base[i]) {
                res ^= base[i];
            }
        }

        return res;
    }

    bool can_create(T val) {
        for(int i = max_log - 1; i >= 0; i--) {
            if(((val >> i) & 1) && base[i]) {
                val ^= base[i];
            }
        }

        return (val == 0);
    }

    vector<T> get_basis() {
        vector<T> res;
        for(int i = 0; i < max_log; i++) {
            if(base[i]) {
                res.push_back(base[i]);
            }
        }
        return res;
    }

    basis<T> merge(basis<T> other) {
        if(max_log < other.max_log) {
            return other.merge(*this);
        }

        basis<T> res = *this;
        for(auto x: other.base) {
            if(x) {
                res.add(x);
            }
        }
        return res;
    }
};

const int MAXLOG = 20;

int n;
vector<vector<int>> adj;
vector<int> a;

void read() {
    cin >> n;
    adj.assign(n, {});
    a.resize(n);
    cin >> a;
    for(int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
}

vector<basis<int>> bs;
vector<pair<pair<int, int>, int>> queries;
vector<int> ans, sz, marker;
vector<bool> used;

int get_size(int v, int p) {
    sz[v] = 1;
    for(int u: adj[v]) {
        if(u != p && !used[u]) {
            sz[v] += get_size(u, v);
        }
    }
    return sz[v];
}

void dfs_basis(int v, int p, basis<int> pref_bs) {
    basis<int> nw_bs = pref_bs;
    nw_bs.add(a[v]);
    bs[v] = nw_bs;
    for(auto u: adj[v]) {
        if(u != p && !used[u]) {
            dfs_basis(u, v, nw_bs);
        }
    }
}

void dfs_marker(int v, int p, int mark) {
    marker[v] = mark;
    for(int u: adj[v]) {
        if(u != p && !used[u]) {
            dfs_marker(u, v, mark);
        }
    }
}

void decompose(int u, vector<int> qidx) {
    get_size(u, -1);
    int n = sz[u], centroid = u, p = -1;
    while(true) {
        int nxt = -1;
        for(int v: adj[centroid]) {
            if(v != p && !used[v] && sz[v] > n / 2) {
                nxt = v;
                break;
            }
        }

        if(nxt != -1) {
            p = centroid;
            centroid = nxt;
        } else {
            break;
        }
    }

    used[centroid] = true;
    basis<int> pref_bs;
    pref_bs.init(MAXLOG);
    pref_bs.add(a[centroid]);
    bs[centroid] = pref_bs;

    int mark = 0;
    vector<vector<int>> children_queries(adj[centroid].size());
    for(int v: adj[centroid]) {
        if(!used[v]) {
            dfs_marker(v, centroid, mark);
            dfs_basis(v, centroid, pref_bs);
        }
        mark++;
    }

    marker[centroid] = mark;
    for(int idx: qidx) {
        auto [xy, k] = queries[idx];
        auto [x, y] = xy;
        if(marker[x] == marker[y] && x != centroid) {
            children_queries[marker[x]].push_back(idx);
        } else {
            basis<int> q_basis = bs[x].merge(bs[y]);
            if(q_basis.can_create(k)) {
                ans[idx] = 1;
            } else {
                ans[idx] = 0;
            }
        }
    }

    int i = 0;
    for(int v: adj[centroid]) {
        if(!used[v] && children_queries[i].size() > 0) {
            decompose(v, children_queries[i]);
        }
        i++;
    }
}

void solve() {
    bs.resize(n);
    sz.resize(n);
    used.assign(n, false);
    marker.assign(n, 0);

    int q;
    cin >> q;
    queries.resize(q);
    for(int i = 0; i < q; i++) {
        int x, y, k;
        cin >> x >> y >> k;
        x--, y--;
        queries[i] = {{x, y}, k};
    }

    vector<int> qidx(q);
    ans.resize(q);
    iota(qidx.begin(), qidx.end(), 0);
    decompose(0, qidx);

    for(int i = 0; i < q; i++) {
        if(ans[i] == 1) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
