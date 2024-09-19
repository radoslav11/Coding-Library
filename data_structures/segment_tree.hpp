#include <bits/stdc++.h>
using namespace std;

// Motivated by atcoder library, but adapted to my style. All ranges are
// inclusive in this implementation.
// https://atcoder.github.io/ac-library/production/document_en/segtree.html

template<class T, T (*merge)(T, T), T (*e)()>
class SegmentTree {
  private:
    int n, size;
    vector<T> tr;

    void pull(int x) { tr[x] = merge(tr[2 * x], tr[2 * x + 1]); }

  public:
    SegmentTree() { init(vector<T>()); }
    SegmentTree(int _n) { init(vector<T>(_n, e())); }
    SegmentTree(const vector<T> &_a) { init(_a); }

    void init(const vector<T> &_a) {
        n = _a.size();
        size = 1;
        while(size < n) {
            size <<= 1;
        }

        tr.assign(2 * size, e());
        for(int i = 0; i < n; i++) {
            tr[size + i] = _a[i];
        }
        for(int i = size - 1; i > 0; i--) {
            pull(i);
        }
    }

    void update(int pos, T val) {
        pos += size;
        tr[pos] = val;
        for(pos >>= 1; pos > 0; pos >>= 1) {
            pull(pos);
        }
    }

    T get_pos(int pos) { return tr[pos + size]; }

    T query(int l, int r) {
        T ansl = e(), ansr = e();
        for(l += size, r += size + 1; l < r; l >>= 1, r >>= 1) {
            if(l & 1) {
                ansl = merge(ansl, tr[l++]);
            }
            if(r & 1) {
                ansr = merge(tr[--r], ansr);
            }
        }
        return merge(ansl, ansr);
    }

    T query_all() { return tr[1]; }

    template<bool (*f)(T)>
    int max_right(int l) const {
        return max_right(l, [](T x) { return f(x); });
    }
    template<class F>
    int max_right(int l, F f) const {
        if(l == n) {
            return n;
        }

        l += size;
        T sm = e();
        do {
            while(l % 2 == 0) {
                l >>= 1;
            }
            if(!f(merge(sm, tr[l]))) {
                while(l < size) {
                    l = (2 * l);
                    if(f(merge(sm, tr[l]))) {
                        sm = merge(sm, tr[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = merge(sm, tr[l]);
            l++;
        } while((l & -l) != l);
        return n;
    }

    template<bool (*f)(T)>
    int min_left(int r) const {
        return min_left(r, [](T x) { return f(x); });
    }
    template<class F>
    int min_left(int r, F f) const {
        if(r == -1) {
            return 0;
        }

        r += size + 1;
        T sm = e();
        do {
            r--;
            while(r > 1 && (r % 2)) {
                r >>= 1;
            }
            if(!f(merge(tr[r], sm))) {
                while(r < size) {
                    r = (2 * r + 1);
                    if(f(merge(tr[r], sm))) {
                        sm = merge(tr[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = merge(tr[r], sm);
        } while((r & -r) != r);
        return 0;
    }
};

// int min_custom(int a, int b) { return min(a, b); }
// int min_e() { return INT_MAX; }
// int max_custom(int a, int b) { return max(a, b); }
// int max_e() { return INT_MIN; }
