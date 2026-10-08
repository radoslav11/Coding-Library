#ifndef NTT_HPP
#define NTT_HPP

#include <bits/stdc++.h>
using namespace std;

// Number theoretic transform modulo a prime mod = c * 2^k + 1 < 2^30.
// Default is 998244353 = 119 * 2^23 + 1 with primitive root 3.
// Roots are precomputed once and the data is kept as uint32_t in Montgomery
// friendly form. The forward pass outputs bit-reversed order and the backward
// pass takes it, so mult and inverse need no bit reversal.
// The loops auto-vectorize under #pragma GCC optimize("O3") and
// #pragma GCC target("avx2") (put after the includes), about 1.3x faster.
// inverse gives the power series 1 / f by Newton iteration in O(n log n),
// for an NTT prime (NTT::inverse) or any prime mod (NTTAnyMod::inverse).

// a^p modulo mod, shared by everything below.
int64_t mod_pow(int64_t a, int64_t p, int64_t mod) {
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

// First len coefficients of the power series 1 / f modulo a prime mod, values
// of f in [0, mod) and f[0] != 0. Shared by NTT::inverse and
// NTTAnyMod::inverse. Schoolbook for the first 32 coefficients, then Newton:
// if g = 1 / f mod x^k, then g - g * (f * g - 1) = 1 / f mod x^2k, where
// f * g - 1 vanishes below k. correction(g, k) must return a vector whose
// entries [k, 2k) are those of g * (f * g - 1).
template<class F>
vector<int64_t> newton_inverse(
    const vector<int64_t>& f, int len, int64_t mod, F correction
) {
    vector<int64_t> g(min(len, 32), 0);
    int64_t inv0 = mod_pow(f[0], mod - 2, mod);
    for(int i = 0; i < (int)g.size(); i++) {
        int64_t s = i == 0;
        for(int j = 1; j <= min(i, (int)f.size() - 1); j++) {
            s = (s + (mod - f[j]) * g[i - j]) % mod;
        }
        g[i] = s * inv0 % mod;
    }

    for(int k = (int)g.size(); k < len; k <<= 1) {
        auto c = correction(g, k);
        g.resize(2 * k);
        for(int i = k; i < 2 * k; i++) {
            g[i] = c[i] ? mod - c[i] : 0;
        }
    }

    g.resize(len);
    return g;
}

class NTTAnyMod;

template<int64_t mod = 998244353, int64_t root = 3>
class NTT {
  private:
    friend class NTTAnyMod;
    static_assert(mod < (1 << 30), "Montgomery form needs mod < 2^30");

    // Montgomery arithmetic with R = 2^32, which needs mod < 2^30. mul(a, b)
    // is a * b / R for a * b < mod * R, lazily reduced into [0, 2 * mod).
    // Transforms keep plain values in [0, 2 * mod) and twiddles times R, so
    // mul(x, rt[s]) is just x * w.
    static constexpr uint32_t m2 = 2 * mod;
    static constexpr uint32_t r1 = (1ULL << 32) % mod;
    static constexpr uint32_t r2 = (uint64_t)r1 * r1 % mod;
    static constexpr uint32_t neg_inv = [] {
        uint32_t x = mod;
        for(int i = 0; i < 5; i++) {
            x *= 2 - (uint32_t)mod * x;
        }
        return -x;
    }();

    static uint32_t mul(uint32_t a, uint32_t b) {
        uint64_t t = (uint64_t)a * b;
        return (t + (uint64_t)((uint32_t)t * neg_inv) * mod) >> 32;
    }

    static uint32_t add(uint32_t a, uint32_t b) {
        return a + b >= m2 ? a + b - m2 : a + b;
    }

    // rt[s] is the twiddle of block s, the same at every level: w^rev(s) for
    // w of order 2^k and rev reversing the k - 1 low bits, so the blocks of
    // size 2^l use the roots of order 2^l in bit-reversed order. Going from
    // s - 1 to s only changes the low ctz(s) + 1 bits of rev, which gives
    // rt[s] = rt[s - 1] * step[ctz(s)]. irt[s] = 1 / rt[s].
    inline static vector<uint32_t> rt = {r1}, irt = {r1};

    static void prepare_roots(int n) {
        int s = (int)rt.size();
        if(s >= n / 2) {
            return;
        }

        vector<uint32_t> step, istep;
        int64_t prod = 1, iprod = 1;
        for(int i = 0; (2 << i) < n; i++) {
            int64_t w = mod_pow(root, (mod - 1) >> (i + 2), mod);
            int64_t iw = mod_pow(w, mod - 2, mod);
            step.push_back(w * prod % mod * r1 % mod);
            istep.push_back(iw * iprod % mod * r1 % mod);
            prod = prod * iw % mod;
            iprod = iprod * w % mod;
        }

        rt.resize(n / 2);
        irt.resize(n / 2);
        for(; s < n / 2; s++) {
            int i = __builtin_ctz(s);
            rt[s] = mul(rt[s - 1], step[i]) % mod;
            irt[s] = mul(irt[s - 1], istep[i]) % mod;
        }
    }

    // Natural order in, transform in bit-reversed order out.
    static void dif(vector<uint32_t>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = n >> 1; k > 0; k >>= 1) {
            for(int i = 0, s = 0; i < n; i += 2 * k, s++) {
                for(int j = i; j < i + k; j++) {
                    uint32_t u = a[j], v = mul(a[j + k], rt[s]);
                    a[j] = add(u, v);
                    a[j + k] = add(u, m2 - v);
                }
            }
        }
    }

    // Bit-reversed order in, n times the inverse transform in natural order
    // out, so dit(dif(a)) = n * a.
    static void dit(vector<uint32_t>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0, s = 0; i < n; i += 2 * k, s++) {
                for(int j = i; j < i + k; j++) {
                    uint32_t u = a[j], v = a[j + k];
                    a[j] = add(u, v);
                    a[j + k] = mul(u + m2 - v, irt[s]);
                }
            }
        }
    }

    // Copy of the first n values of a reduced into [0, mod), padded with
    // zeros to size n.
    template<class T>
    static vector<uint32_t> reduce(const vector<T>& a, int n) {
        vector<uint32_t> f(n, 0);
        for(int i = 0; i < min((int)a.size(), n); i++) {
            int64_t x = a[i] % mod;
            f[i] = x < 0 ? x + mod : x;
        }
        return f;
    }

    // Transform of size n (power of two), in bit-reversed order.
    template<class T>
    static vector<uint32_t> spectrum(const vector<T>& a, int n) {
        vector<uint32_t> f = reduce(a, n);
        dif(f);
        return f;
    }

    // Replaces the spectrum fa by the cyclic convolution of size n of the
    // two inputs, in natural order. fb may be fa itself (squaring).
    static void cyclic(vector<uint32_t>& fa, const vector<uint32_t>& fb) {
        int n = (int)fa.size();
        // The two mul calls divide by R^2 and dit multiplies by n.
        uint32_t c = mod_pow(n, mod - 2, mod) * r2 % mod;
        for(int i = 0; i < n; i++) {
            fa[i] = mul(mul(fa[i], fb[i]), c);
        }

        dit(fa);
        for(auto& x: fa) {
            x = x >= mod ? x - mod : x;
        }
    }

  public:
    // In-place transform in natural order, size of a must be a power of two.
    static void ntt(vector<int64_t>& a, bool invert) {
        int n = (int)a.size();
        vector<uint32_t> f = reduce(a, n);
        if(!invert) {
            dif(f);
        }

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

        uint32_t c = r1;
        if(invert) {
            dit(f);
            c = mod_pow(n, mod - 2, mod) * r1 % mod;
        }

        for(int i = 0; i < n; i++) {
            a[i] = mul(f[i], c) % mod;
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

        if(min((int)a.size(), (int)b.size()) <= 32) {
            vector<uint32_t> fa = reduce(a, n), fb = reduce(b, n);
            vector<int64_t> res(res_size, 0);
            for(int i = 0; i < (int)a.size(); i++) {
                for(int j = 0; j < (int)b.size(); j++) {
                    res[i + j] = (res[i + j] + (uint64_t)fa[i] * fb[j]) % mod;
                }
            }
            return res;
        }

        // Squaring (a == b) needs one forward transform instead of two.
        vector<uint32_t> fa = spectrum(a, n);
        cyclic(fa, a == b ? fa : spectrum(b, n));
        return vector<int64_t>(fa.begin(), fa.begin() + res_size);
    }

    // First len coefficients of 1 / f, values in [0, mod), f[0] != 0.
    // About 5 transforms of size 2k per doubling step k -> 2k.
    static vector<int64_t> inverse(const vector<int64_t>& f, int len) {
        return newton_inverse(
            f, len, mod, [&](const vector<int64_t>& g, int k) {
                // With deg g < k, the cyclic product of size 2k is exact on
                // [k, 2k), the wrap-around only reaches [0, k).
                vector<uint32_t> sg = spectrum(g, 2 * k);
                auto step = [&](const auto& a) {
                    vector<uint32_t> c = spectrum(a, 2 * k);
                    cyclic(c, sg);
                    fill(c.begin(), c.begin() + k, 0);
                    return c;
                };
                return step(step(f));
            }
        );
    }
};

// Exact convolution modulo an arbitrary mod < 2^31 with three NTT primes and
// CRT. Exact while n * mod^2 < p1 * p2 * p3 ~ 5.9e25 and n <= 2^24.

class NTTAnyMod {
  private:
    static constexpr int64_t p1 = 167772161, p2 = 469762049, p3 = 754974721;
    using N1 = NTT<p1, 3>;
    using N2 = NTT<p2, 3>;
    using N3 = NTT<p3, 11>;

    // Combines residues mod p1, p2, p3 of the same exact values into mod.
    template<class T>
    static vector<int64_t> crt(
        const vector<T>& c1, const vector<T>& c2, const vector<T>& c3,
        int64_t mod
    ) {
        static const int64_t inv_p1 = mod_pow(p1, p2 - 2, p2);
        static const int64_t inv_p12 = mod_pow(p1 * p2 % p3, p3 - 2, p3);
        const int64_t p12 = p1 * p2 % mod;
        vector<int64_t> res(c1.size());
        for(int i = 0; i < (int)c1.size(); i++) {
            // x = c1 + p1 * k1 + p1 * p2 * k2 with k1 < p2, k2 < p3.
            int64_t k1 = (c2[i] + p2 - c1[i]) * inv_p1 % p2;
            int64_t x = c1[i] + p1 * k1;
            int64_t k2 = (c3[i] - x % p3 + p3) * inv_p12 % p3;
            res[i] = (x + p12 * k2) % mod;
        }
        return res;
    }

  public:
    // Values must be in [0, mod).
    static vector<int64_t> mult_mod(
        const vector<int64_t>& a, const vector<int64_t>& b, int64_t mod
    ) {
        return crt(N1::mult(a, b), N2::mult(a, b), N3::mult(a, b), mod);
    }

    // First len coefficients of 1 / f for a prime mod < 2^31, values in
    // [0, mod), f[0] != 0. Same Newton step as NTT::inverse, the spectra of
    // g are shared by both products and each product goes through CRT.
    static vector<int64_t> inverse(
        const vector<int64_t>& f, int len, int64_t mod
    ) {
        return newton_inverse(
            f, len, mod, [&](const vector<int64_t>& g, int k) {
                vector<uint32_t> s1 = N1::spectrum(g, 2 * k);
                vector<uint32_t> s2 = N2::spectrum(g, 2 * k);
                vector<uint32_t> s3 = N3::spectrum(g, 2 * k);
                auto step = [&](const vector<int64_t>& a) {
                    vector<uint32_t> c1 = N1::spectrum(a, 2 * k);
                    vector<uint32_t> c2 = N2::spectrum(a, 2 * k);
                    vector<uint32_t> c3 = N3::spectrum(a, 2 * k);
                    N1::cyclic(c1, s1);
                    N2::cyclic(c2, s2);
                    N3::cyclic(c3, s3);
                    vector<int64_t> c = crt(c1, c2, c3, mod);
                    fill(c.begin(), c.begin() + k, 0);
                    return c;
                };
                return step(step(f));
            }
        );
    }
};

// vector<int64_t> c = NTT<>::mult(a, b);
// NTT<>::ntt(a, false); ... NTT<>::ntt(a, true);
// vector<int64_t> g = NTT<>::inverse(f, len);  // f * g = 1 mod x^len
// Other primes: NTT<167772161, 3>, NTT<469762049, 3>, NTT<754974721, 11>.
// vector<int64_t> c = NTTAnyMod::mult_mod(a, b, mod);  // any mod < 2^31
// vector<int64_t> g = NTTAnyMod::inverse(f, len, mod); // prime mod < 2^31

#endif  // NTT_HPP
