#include <bits/stdc++.h>
using namespace std;

template<class T, int num_bits>
class xor_basis {
  public:
    int sz;
    vector<T*> base;

    void clear() {
        sz = 0;
        base.assign(num_bits, nullptr);
    }
    xor_basis() { clear(); }

    void add(T val) {
        for(int i = num_bits - 1; i >= 0; i--) {
            if(get_bit(val, i)) {
                if(!base[i]) {
                    base[i] = new T(val);
                    sz++;
                    return;
                } else {
                    val = xor_op(val, *base[i]);
                }
            }
        }
    }

    inline int size() { return sz; }

    T max_xor() {
        T res = T();
        for(int i = num_bits - 1; i >= 0; i--) {
            if(!get_bit(res, i) && base[i]) {
                res = xor_op(res, *base[i]);
            }
        }

        return res;
    }

    bool can_create(T val) {
        for(int i = num_bits - 1; i >= 0; i--) {
            if(get_bit(val, i) && base[i]) {
                val = xor_op(val, *base[i]);
            }
        }
        return is_zero(val);
    }

    vector<T> get_basis() {
        vector<T> res;
        for(int i = 0; i < num_bits; i++) {
            if(!base[i]) {
                res.push_back(*base[i]);
            }
        }
        return res;
    }

    xor_basis<T, num_bits> merge(const xor_basis<T, num_bits>& other) {
        if(sz < other.size()) {
            return other.merge(*this);
        }

        xor_basis<T, num_bits> res = *this;
        for(auto x: other.base) {
            if(x) {
                res.add(*x);
            }
        }
        return res;
    }

  private:
    // Helper functions for different types

    // For integral types
    template<typename U = T>
    typename enable_if<is_integral<U>::value, bool>::type get_bit(
        const T& val, int pos
    ) const {
        return (val >> pos) & 1;
    }

    template<typename U = T>
    typename enable_if<is_integral<U>::value, bool>::type is_zero(const T& val
    ) const {
        return val == 0;
    }

    template<typename U = T>
    typename enable_if<is_integral<U>::value, T>::type xor_op(
        const T& a, const T& b
    ) const {
        return a ^ b;
    }

    // For bitset
    template<size_t N, typename U = T>
    typename enable_if<is_same<U, bitset<N>>::value, bool>::type get_bit(
        const bitset<N>& val, int pos
    ) const {
        return val[pos];
    }

    template<size_t N, typename U = T>
    typename enable_if<is_same<U, bitset<N>>::value, bool>::type is_zero(
        const bitset<N>& val
    ) const {
        return val.none();
    }

    template<size_t N, typename U = T>
    typename enable_if<is_same<U, bitset<N>>::value, bitset<N>>::type xor_op(
        const bitset<N>& a, const bitset<N>& b
    ) const {
        return a ^ b;
    }
};
