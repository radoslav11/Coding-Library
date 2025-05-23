#include <bits/stdc++.h>
using namespace std;

template<class T>
class MonoidMin {
  public:
    struct MinVal {
        T val;
        bool operator==(const MinVal& other) const { return val == other.val; }
        bool operator!=(const MinVal& other) const { return !(*this == other); }
    };

    struct AddLazy {
        T add_val;
        bool operator==(const AddLazy& other) const {
            return add_val == other.add_val;
        }
        bool operator!=(const AddLazy& other) const {
            return !(*this == other);
        }
    };

    static MinVal merge(MinVal a, MinVal b) { return {min(a.val, b.val)}; }

    static MinVal e() { return {numeric_limits<T>::max()}; }

    static MinVal lazy_apply(AddLazy f, MinVal x) {
        return {x.val + f.add_val};
    }

    static AddLazy lazy_merge(AddLazy a, AddLazy b) {
        return {a.add_val + b.add_val};
    }

    static AddLazy lazy_init(MinVal _) { return {0}; }
};

// SegmentTreeLazy<
//     MonoidMin<int>::MinVal,
//     MonoidMin<int>::merge,
//     MonoidMin<int>::e,
//     MonoidMin<int>::AddLazy,
//     MonoidMin<int>::lazy_apply,
//     MonoidMin<int>::lazy_merge,
//     MonoidMin<int>::lazy_init>

template<class T>
class MonoidSum {
  public:
    using Sum = T;
    using AddLazy = T;

    static Sum merge(Sum a, Sum b) { return a + b; }

    static Sum e() { return 0; }

    static Sum lazy_apply(AddLazy f, Sum x) { return f + x; }

    static AddLazy lazy_merge(AddLazy a, AddLazy b) { return a + b; }

    static AddLazy lazy_init(Sum _) { return 0; }

    static Sum inverse(Sum x) { return -x; }
};

// SegmentTreeLazy<
//     MonoidSum<int>::Sum,
//     MonoidSum<int>::merge,
//     MonoidSum<int>::e,
//     MonoidSum<int>::AddLazy,
//     MonoidSum<int>::lazy_apply,
//     MonoidSum<int>::lazy_merge,
//     MonoidSum<int>::lazy_init>

template<class T>
class MonoidMinWithCount {
  public:
    struct MinWithCount {
        T min_val;
        int count;

        bool operator==(const MinWithCount& other) const {
            return min_val == other.min_val && count == other.count;
        }
        bool operator!=(const MinWithCount& other) const {
            return !(*this == other);
        }
    };

    struct AddLazy {
        T add_val;
        bool operator==(const AddLazy& other) const {
            return add_val == other.add_val;
        }
        bool operator!=(const AddLazy& other) const {
            return !(*this == other);
        }
    };

    static MinWithCount merge(MinWithCount a, MinWithCount b) {
        if(a.min_val < b.min_val) {
            return a;
        }
        if(b.min_val < a.min_val) {
            return b;
        }
        return {a.min_val, a.count + b.count};
    }

    static MinWithCount e() { return {numeric_limits<T>::max(), 0}; }

    static MinWithCount lazy_apply(AddLazy f, MinWithCount x) {
        return {x.min_val + f.add_val, x.count};
    }

    static AddLazy lazy_merge(AddLazy a, AddLazy b) {
        return {a.add_val + b.add_val};
    }
    static AddLazy lazy_init(MinWithCount _) { return {0}; }
};

// SegmentTreeLazy<
//     MonoidMinWithCount<int>::MinWithCount,
//     MonoidMinWithCount<int>::merge,
//     MonoidMinWithCount<int>::e,
//     MonoidMinWithCount<int>::AddLazy,
//     MonoidMinWithCount<int>::lazy_apply,
//     MonoidMinWithCount<int>::lazy_merge,
//     MonoidMinWithCount<int>::lazy_init>

template<class T>
class MonoidSumArithmeticProgression {
    struct Sum {
        T val;
        T l, r;
        bool operator==(const Sum& other) const {
            return val == other.val && l == other.l && r == other.r;
        }
        bool operator!=(const Sum& other) const { return !(*this == other); }
    };
    struct AddLazy {
        T alpha;
        T beta;
        T l_border;
        bool operator==(const AddLazy& other) const {
            return alpha == other.alpha && beta == other.beta &&
                   l_border == other.l_border;
        }
        bool operator!=(const AddLazy& other) const {
            return !(*this == other);
        }
    };

    static Sum merge(Sum a, Sum b) {
        return {
            a.val + b.val,
            a.l,
            b.r,
        };
    }
    static Sum e() { return {0, 0}; }
    static Sum lazy_apply(AddLazy f, Sum x) {
        T delta = x.l - f.l_border;
        T len = x.r - x.l + 1;
        x.val +=
            (delta * f.beta + f.alpha) * len + f.beta * len * (len + 1) / 2;
        return x;
    }
    static AddLazy lazy_merge(AddLazy a, AddLazy b) {
        T delta = b.l_border - a.l_border;
        b.alpha += a.alpha + delta * a.beta;
        b.beta += a.beta;
        return b;
    }
    static AddLazy lazy_init(Sum node) { return {0, 0, node.l}; }
};

// SegmentTreeLazy<
//     MonoidSumArithmeticProgression<int64_t>::Sum,
//     MonoidSumArithmeticProgression<int64_t>::merge,
//     MonoidSumArithmeticProgression<int64_t>::e,
//     MonoidSumArithmeticProgression<int64_t>::AddLazy,
//     MonoidSumArithmeticProgression<int64_t>::lazy_apply,
//     MonoidSumArithmeticProgression<int64_t>::lazy_merge,
//     MonoidSumArithmeticProgression<int64_t>::lazy_init>
