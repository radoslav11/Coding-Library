#ifndef MOD_INT_HPP
#define MOD_INT_HPP

#include <bits/stdc++.h>
using namespace std;

// Modular integer for a prime mod (inv() uses Fermat's little theorem).
// The constructor does not reduce, so pass values in [0, mod).

template<class T>
T pw(T a, int pw) {
    T ret(1);
    while(pw) {
        if(pw & 1) {
            ret *= a;
        }
        a *= a;
        pw >>= 1;
    }
    return ret;
}

template<unsigned mod = 998244353>
class modint_t {
  private:
    unsigned x;

  public:
    modint_t() { x = 0; }
    modint_t(unsigned _x) { x = _x; }
    operator unsigned() { return x; }

    modint_t operator+=(const modint_t& m) {
        x = (x + m.x >= mod ? x + m.x - mod : x + m.x);
        return *this;
    }
    modint_t operator-=(const modint_t& m) {
        x = (x < m.x ? x - m.x + mod : x - m.x);
        return *this;
    }
    modint_t operator*=(const modint_t& m) {
        x = 1ULL * x * m.x % mod;
        return *this;
    }

    modint_t operator+(const modint_t& m) const { return modint_t(*this) += m; }
    modint_t operator-(const modint_t& m) const { return modint_t(*this) -= m; }
    modint_t operator*(const modint_t& m) const { return modint_t(*this) *= m; }

    modint_t inv() { return pw(modint_t(*this), mod - 2); }
};

// using mint = modint_t<998244353>;
// mint a = mint(x % mod), b = 5;
// mint c = (a + b) * b.inv() - a;
// cout << (unsigned)c << '\n';
// mint d = pw(a, k);

#endif  // MOD_INT_HPP
