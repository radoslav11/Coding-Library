#include <bits/stdc++.h>
using namespace std;

template<class T, int num_bits>
class xor_basis {
  public:
    int sz;
    vector<T> base;

    void clear() {
        sz = 0;
        base.assign(num_bits, 0);
    }
    xor_basis() { clear(); }

    void add(T val) {
        for(int i = num_bits - 1; i >= 0; i--) {
            if((val >> i) & 1) {
                if(!base[i]) {
                    base[i] = val;
                    sz++;
                    return;
                } else {
                    val ^= base[i];
                }
            }
        }
    }

    inline int size() { return sz; }

    T max_xor() {
        T res = 0;
        for(int i = num_bits - 1; i >= 0; i--) {
            if(!((res >> i) & 1) && base[i]) {
                res ^= base[i];
            }
        }

        return res;
    }

    bool can_create(T val) {
        for(int i = num_bits - 1; i >= 0; i--) {
            if(((val >> i) & 1) && base[i]) {
                val ^= base[i];
            }
        }

        return (val == 0);
    }

    vector<T> get_basis() {
        vector<T> res;
        for(int i = 0; i < num_bits; i++) {
            if(base[i]) {
                res.push_back(base[i]);
            }
        }
        return res;
    }

    xor_basis<T, num_bits> merge(xor_basis<T, num_bits> other) {
        if(sz < other.size()) {
            return other.merge(*this);
        }

        xor_basis<T, num_bits> res = *this;
        for(auto x: other.base) {
            if(x) {
                res.add(x);
            }
        }
        return res;
    }
};
