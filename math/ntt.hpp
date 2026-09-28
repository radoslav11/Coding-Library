#ifndef NTT_HPP
#define NTT_HPP

#include <bits/stdc++.h>
using namespace std;

// Number theoretic transform modulo a prime mod = c * 2^k + 1 < 2^31.
// Default is 998244353 = 119 * 2^23 + 1 with primitive root 3.
// Roots are precomputed once and the data is kept as uint32_t. mult does a
// decimation in frequency forward pass (bit-reversed output) and a decimation
// in time backward pass (bit-reversed input), so it needs no bit reversal.

template<int64_t mod = 998244353, int64_t root = 3>
class NTT {
  private:
    // rt[k + j] = w^j for w a primitive (2k)-th root of unity, j < k.
    inline static vector<uint32_t> rt = {1, 1};

    static uint32_t mul(uint32_t a, uint32_t b) {
        return (uint64_t)a * b % mod;
    }

    static int64_t pw(int64_t a, int64_t p) {
        int64_t res = 1;
        a %= mod;
        while(p) {
            if(p & 1) {
                res = res * a % mod;
            }
            a = a * a % mod;
            p >>= 1;
        }
        return res;
    }

    static void prepare_roots(int n) {
        for(int k = (int)rt.size(); k < n; k <<= 1) {
            rt.resize(2 * k);
            uint32_t z = pw(root, (mod - 1) / (2 * k));
            for(int i = k; i < 2 * k; i++) {
                rt[i] = i & 1 ? mul(rt[i / 2], z) : rt[i / 2];
            }
        }
    }

    // Natural order in, transform in bit-reversed order out.
    static void dif(vector<uint32_t>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = n >> 1; k > 0; k >>= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = i; j < i + k; j++) {
                    uint32_t u = a[j], v = a[j + k];
                    a[j] = u + v >= mod ? u + v - mod : u + v;
                    a[j + k] = mul(u + (uint32_t)mod - v, rt[k + j - i]);
                }
            }
        }
    }

    // Bit-reversed order in, transform in natural order out.
    static void dit(vector<uint32_t>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = i; j < i + k; j++) {
                    uint32_t u = a[j], v = mul(a[j + k], rt[k + j - i]);
                    a[j] = u + v >= mod ? u + v - mod : u + v;
                    a[j + k] = u >= v ? u - v : u + (uint32_t)mod - v;
                }
            }
        }
    }

    // Copy of a reduced into [0, mod), padded with zeros to size n.
    static vector<uint32_t> reduce(const vector<int64_t>& a, int n) {
        vector<uint32_t> f(n, 0);
        for(int i = 0; i < (int)a.size(); i++) {
            f[i] = (a[i] % mod + mod) % mod;
        }
        return f;
    }

  public:
    // In-place transform in natural order, size of a must be a power of two.
    // The inverse is the forward transform read at -i, divided by n.
    static void ntt(vector<int64_t>& a, bool invert) {
        int n = (int)a.size();
        vector<uint32_t> f = reduce(a, n);
        for(int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for(; j & bit; bit >>= 1) {
                j ^= bit;
            }
            j ^= bit;
            if(i < j) {
                swap(f[i], f[j]);
            }
        }

        dit(f);
        uint32_t inv_n = pw(n, mod - 2);
        for(int i = 0; i < n; i++) {
            a[i] = invert ? mul(f[(n - i) & (n - 1)], inv_n) : f[i];
        }
    }

    // Any int64_t values are fine, they are reduced mod first.
    static vector<int64_t> mult(
        const vector<int64_t>& a, const vector<int64_t>& b
    ) {
        if(a.empty() || b.empty()) {
            return {};
        }

        int res_size = (int)a.size() + (int)b.size() - 1, n = 1;
        while(n < res_size) {
            n <<= 1;
        }

        vector<uint32_t> fa = reduce(a, n), fb = reduce(b, n);
        vector<int64_t> res(res_size, 0);
        if(min((int)a.size(), (int)b.size()) <= 32) {
            for(int i = 0; i < (int)a.size(); i++) {
                for(int j = 0; j < (int)b.size(); j++) {
                    res[i + j] = (res[i + j] + mul(fa[i], fb[j])) % mod;
                }
            }
            return res;
        }

        dif(fa);
        dif(fb);
        uint32_t inv_n = pw(n, mod - 2);
        for(int i = 0; i < n; i++) {
            fa[i] = mul(mul(fa[i], fb[i]), inv_n);
        }

        // dit(dif(x)) is n * x read at -i.
        dit(fa);
        for(int i = 0; i < res_size; i++) {
            res[i] = fa[(n - i) & (n - 1)];
        }
        return res;
    }
};

// Exact convolution modulo an arbitrary mod < 2^31 with three NTT primes and
// CRT. Exact while n * mod^2 < p1 * p2 * p3 ~ 5.9e25 and n <= 2^24.

class NTTAnyMod {
  private:
    static int64_t pw(int64_t a, int64_t p, int64_t mod) {
        int64_t res = 1;
        a %= mod;
        while(p) {
            if(p & 1) {
                res = res * a % mod;
            }
            a = a * a % mod;
            p >>= 1;
        }
        return res;
    }

  public:
    // Values must be in [0, mod).
    static vector<int64_t> mult_mod(
        const vector<int64_t>& a, const vector<int64_t>& b, int64_t mod
    ) {
        const int64_t p1 = 167772161, p2 = 469762049, p3 = 754974721;
        vector<int64_t> c1 = NTT<p1, 3>::mult(a, b);
        vector<int64_t> c2 = NTT<p2, 3>::mult(a, b);
        vector<int64_t> c3 = NTT<p3, 11>::mult(a, b);
        const int64_t inv_p1 = pw(p1, p2 - 2, p2);
        const int64_t inv_p12 = pw(p1 * p2 % p3, p3 - 2, p3);
        const int64_t p12 = p1 * p2 % mod;
        vector<int64_t> res(c1.size());
        for(int i = 0; i < (int)c1.size(); i++) {
            // x = c1 + p1 * k1 + p1 * p2 * k2 with k1 < p2, k2 < p3.
            int64_t k1 = (c2[i] - c1[i] + p2) * inv_p1 % p2;
            int64_t x = c1[i] + p1 * k1;
            int64_t k2 = (c3[i] - x % p3 + p3) * inv_p12 % p3;
            res[i] = (x + p12 * k2) % mod;
        }
        return res;
    }
};

// vector<int64_t> c = NTT<>::mult(a, b);
// NTT<>::ntt(a, false); ... NTT<>::ntt(a, true);
// Other primes: NTT<167772161, 3>, NTT<469762049, 3>, NTT<754974721, 11>.
// vector<int64_t> c = NTTAnyMod::mult_mod(a, b, mod);  // any mod < 2^31

#endif  // NTT_HPP
