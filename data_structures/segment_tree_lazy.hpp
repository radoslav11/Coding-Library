#include <bits/stdc++.h>
using namespace std;

// Motivated by atcoder library, but adapted to my style. All ranges are
// inclusive in this implementation.
// https://atcoder.github.io/ac-library/production/document_en/segtree.html

template<
    class T, T (*merge)(T, T), T (*e)(), class G, T (*lazy_apply)(G, T),
    G (*lazy_merge)(G, G), G (*id)()>
class segment_tree_lazy {
  private:
    int n, size, log_size;
    vector<T> tr;
    vector<G> lazy;

    void pull(int x) { tr[x] = merge(tr[2 * x], tr[2 * x + 1]); }

    void push_one(int x, G f) {
        tr[x] = lazy_apply(f, tr[x]);
        if(x < size) {
            lazy[x] = lazy_merge(f, lazy[x]);
        }
    }

    void push(int x) {
        push_one(2 * x, lazy[x]);
        push_one(2 * x + 1, lazy[x]);
        lazy[x] = id();
    }

    void push_all_down(int x) {
        for(int i = log_size; i >= 1; i--) {
            push(x >> i);
        }
    }

    void push_all_open_range(int l, int r) {
        for(int i = log_size; i >= 1; i--) {
            if(((l >> i) << i) != l) {
                push(l >> i);
            }
            if(((r >> i) << i) != r) {
                push((r - 1) >> i);
            }
        }
    }

  public:
    segment_tree_lazy() { init(vector<T>()); }
    segment_tree_lazy(int _n) { init(vector<T>(_n, e())); }
    segment_tree_lazy(const vector<T> &_a) { init(_a); }

    void init(const vector<T> &_a) {
        n = _a.size();
        size = 1;
        log_size = 0;
        while(size < n) {
            size <<= 1;
            log_size++;
        }

        tr.assign(2 * size, e());
        lazy.assign(size, id());
        for(int i = 0; i < n; i++) {
            tr[size + i] = _a[i];
        }
        for(int i = size - 1; i > 0; i--) {
            pull(i);
        }
    }

    void update(int pos, T val) {
        pos += size;
        push_all_down(pos);
        tr[pos] = val;
        for(pos >>= 1; pos > 0; pos >>= 1) {
            pull(pos);
        }
    }

    void apply_lazy(int l, int r, G f) {
        if(l > r) {
            return;
        }

        l += size, r += size + 1;
        push_all_open_range(l, r);
        for(int l0 = l, r0 = r; l0 < r0; l0 >>= 1, r0 >>= 1) {
            if(l0 & 1) {
                push_one(l0++, f);
            }
            if(r0 & 1) {
                push_one(--r0, f);
            }
        }

        for(int i = 1; i <= log_size; i++) {
            if(((l >> i) << i) != l) {
                pull(l >> i);
            }
            if(((r >> i) << i) != r) {
                pull((r - 1) >> i);
            }
        }
    }

    T get_pos(int pos) {
        pos += size;
        push_all_down(pos);
        return tr[pos];
    }

    T query(int l, int r) {
        T ansl = e(), ansr = e();
        l += size, r += size + 1;
        push_all_open_range(l, r);

        for(; l < r; l >>= 1, r >>= 1) {
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
        push_all_down(l);
        T sm = e();
        do {
            while(l % 2 == 0) {
                l >>= 1;
            }
            if(!f(merge(sm, tr[l]))) {
                while(l < size) {
                    push(l);
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
        push_all_down(r - 1);
        T sm = e();
        do {
            r--;
            while(r > 1 && (r % 2)) {
                r >>= 1;
            }
            if(!f(merge(tr[r], sm))) {
                while(r < size) {
                    push(r);
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
