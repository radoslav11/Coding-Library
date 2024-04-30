#include <bits/stdc++.h>

using namespace std;

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

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
