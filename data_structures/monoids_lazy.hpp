#include <bits/stdc++.h>
using namespace std;

template <class T>
class monoid_min {
  public:
    struct min_val {
        T val;
    };

    struct add_lazy {
        T add_val;
    };

    static min_val merge(min_val a, min_val b) {
        return {min(a.val, b.val)};
    }

    static min_val e() { return {numeric_limits<T>::max()}; }

    static min_val lazy_apply(add_lazy f, min_val x) {
        return {x.val + f.add_val};
    }

    static add_lazy lazy_merge(add_lazy a, add_lazy b) {
        return {a.add_val + b.add_val};
    }

    static add_lazy id() { return {0}; }
};

// segment_tree_lazy<
//     monoid_min<int>::min_val, monoid_min<int>::merge, monoid_min<int>::e,
//     monoid_min<int>::add_lazy, monoid_min<int>::lazy_apply, monoid_min<int>::lazy_merge,
//     monoid_min<int>::id>

template <class T>
class monoid_sum {
  public:
    struct sum {
        T val;
    };

    struct add_lazy {
        T add_val;
    };

    static sum merge(sum a, sum b) {
        return {a.val + b.val};
    }

    static sum e() { return {0}; }

    static sum lazy_apply(add_lazy f, sum x) {
        return {x.val + f.add_val};
    }

    static add_lazy lazy_merge(add_lazy a, add_lazy b) {
        return {a.add_val + b.add_val};
    }

    static add_lazy id() { return {0}; }
};

// segment_tree_lazy<
//     monoid_sum<int>::sum, monoid_sum<int>::merge, monoid_sum<int>::e,
//     monoid_sum<int>::add_lazy, monoid_sum<int>::lazy_apply, monoid_sum<int>::lazy_merge,
//     monoid_sum<int>::id>

template<class T>
class monoid_min_with_count {
  public:
    struct min_with_count {
        T min_val;
        int count;
    };

    struct add_lazy {
        T add_val;
    };

    static min_with_count merge(min_with_count a, min_with_count b) {
        if(a.min_val < b.min_val) {
            return a;
        }
        if(b.min_val < a.min_val) {
            return b;
        }
        return {a.min_val, a.count + b.count};
    }

    static min_with_count e() { return {numeric_limits<T>::max(), 0}; }

    static min_with_count lazy_apply(add_lazy f, min_with_count x) {
        return {x.min_val + f.add_val, x.count};
    }

    static add_lazy lazy_merge(add_lazy a, add_lazy b) {
        return {a.add_val + b.add_val};
    }

    static add_lazy id() { return {0}; }
};

// segment_tree_lazy<
//     monoid_min_with_count<int>::min_with_count, monoid_min_with_count<int>::merge,
//     monoid_min_with_count<int>::e, monoid_min_with_count<int>::add_lazy,
//     monoid_min_with_count<int>::lazy_apply, monoid_min_with_count<int>::lazy_merge,
//     monoid_min_with_count<int>::id>
