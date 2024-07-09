#include <bits/stdc++.h>
using namespace std;
const int mod = (int)1e9 + 7;

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

template<unsigned mod>
class modint_t {
  private:
    unsigned x;

  public:
    modint_t() { x = 0; }
    modint_t(unsigned _x) { x = _x; }
    operator unsigned() { return x; }

    bool operator==(const modint_t &m) const { return x == m.x; }
    bool operator!=(const modint_t &m) const { return x != m.x; }

    modint_t operator+=(const modint_t &m) {
        x = (x + m.x >= mod ? x + m.x - mod : x + m.x);
        return *this;
    }
    modint_t operator-=(const modint_t &m) {
        x = (x < m.x ? x - m.x + mod : x - m.x);
        return *this;
    }
    modint_t operator*=(const modint_t &m) {
        x = 1ULL * x * m.x % mod;
        return *this;
    }

    modint_t operator+=(const int32_t m) {
        x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod));
        return *this;
    }
    modint_t operator-=(const int32_t m) {
        x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod));
        return *this;
    }
    modint_t operator*=(const int32_t m) {
        x = 1ULL * x * (m % mod) % mod;
        return *this;
    }

    modint_t operator+=(const int64_t m) {
        x = (x + (m % mod) >= mod ? x + (m % mod) - mod : x + (m % mod));
        return *this;
    }
    modint_t operator-=(const int64_t m) {
        x = (x < (m % mod) ? x - (m % mod) + mod : x - (m % mod));
        return *this;
    }
    modint_t operator*=(const int64_t m) {
        x = 1ULL * x * (m % mod) % mod;
        return *this;
    }

    modint_t operator+(const modint_t &m) const { return modint_t(*this) += m; }
    modint_t operator-(const modint_t &m) const { return modint_t(*this) -= m; }
    modint_t operator*(const modint_t &m) const { return modint_t(*this) *= m; }

    modint_t operator+(const int32_t m) const { return modint_t(*this) += m; }
    modint_t operator-(const int32_t m) const { return modint_t(*this) -= m; }
    modint_t operator*(const int32_t m) const { return modint_t(*this) *= m; }

    modint_t operator+(const int64_t m) const { return modint_t(*this) += m; }
    modint_t operator-(const int64_t m) const { return modint_t(*this) -= m; }
    modint_t operator*(const int64_t m) const { return modint_t(*this) *= m; }

    modint_t inv() { return pw(modint_t(*this), mod - 2); }
};

using mint = modint_t<mod>;

vector<mint> fact, ifact, inv_prec;

void precompute(int bound) {
    fact.resize(bound + 1);
    ifact.resize(bound + 1);
    inv_prec.resize(bound + 1);

    fact[0] = 1;
    for(int i = 1; i <= bound; i++) {
        fact[i] = fact[i - 1] * i;
    }

    ifact[bound] = fact[bound].inv();
    for(int i = bound - 1; i >= 0; i--) {
        ifact[i] = ifact[i + 1] * (i + 1);
    }

    for(int i = 1; i <= bound; i++) {
        inv_prec[i] = fact[i - 1] * ifact[i];
    }
}

mint C(int n, int k) {
    if(n < k || n < 0 || k < 0) {
        return 0;
    }
    return fact[n] * ifact[n - k] * ifact[k];
}

int main() {
    precompute((int)1e6 + 42);
    return 0;
}
