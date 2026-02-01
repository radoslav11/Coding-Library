#include <bits/stdc++.h>
using namespace std;

template<class T, bool maximum = true>
class ConvexHullTrick {
  public:
    static constexpr T inf = is_integral_v<T> ? numeric_limits<T>::max()
                                              : numeric_limits<T>::infinity();

    T div(T a, T b) {
        if constexpr(is_integral_v<T>) {
            return a / b - ((a ^ b) < 0 && a % b);
        } else {
            return a / b;
        }
    }

    struct Line {
        mutable T k, m, p;
        bool operator<(const Line& o) const { return k < o.k; }
        bool operator<(T x) const { return p < x; }
    };

    multiset<Line, less<>> hull;

    using Iter = typename multiset<Line, less<>>::iterator;

    bool intersect(Iter x, Iter y) {
        if(y == hull.end()) {
            return x->p = inf, 0;
        }
        if(x->k == y->k) {
            x->p = x->m > y->m ? inf : -inf;
        } else {
            x->p = div(y->m - x->m, x->k - y->k);
        }
        return x->p >= y->p;
    }

    void add(T k, T m) {
        if constexpr(!maximum) {
            k = -k;
            m = -m;
        }
        auto z = hull.insert({k, m, 0}), y = z++, x = y;
        while(intersect(y, z)) {
            z = hull.erase(z);
        }
        if(x != hull.begin() && intersect(--x, y)) {
            intersect(x, y = hull.erase(y));
        }
        while((y = x) != hull.begin() && (--x)->p >= y->p) {
            intersect(x, hull.erase(y));
        }
    }

    T query(T x) {
        assert(!hull.empty());
        auto l = *hull.lower_bound(x);
        T res = l.k * x + l.m;
        if constexpr(!maximum) {
            res = -res;
        }
        return res;
    }
};
