#ifndef NTT_HPP
#define NTT_HPP

#include <bits/stdc++.h>
using namespace std;

// Number theoretic transform modulo a prime of the form c * 2^k + 1.
// Default is 998244353 = 119 * 2^23 + 1 with primitive root 3.

template<int64_t mod = 998244353, int64_t root = 3>
class NTT {
  private:
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

  public:
    // In-place transform, size of a must be a power of two.
    static void ntt(vector<int64_t>& a, bool invert) {
        int n = (int)a.size();
        for(int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for(; j & bit; bit >>= 1) {
                j ^= bit;
            }
            j ^= bit;
            if(i < j) {
                swap(a[i], a[j]);
            }
        }

        vector<int64_t> w(n / 2 + 1);
        for(int len = 2; len <= n; len <<= 1) {
            int64_t wl = pw(root, (mod - 1) / len);
            if(invert) {
                wl = pw(wl, mod - 2);
            }

            int half = len >> 1;
            w[0] = 1;
            for(int j = 1; j < half; j++) {
                w[j] = w[j - 1] * wl % mod;
            }

            for(int i = 0; i < n; i += len) {
                for(int j = 0; j < half; j++) {
                    int64_t u = a[i + j], v = a[i + j + half] * w[j] % mod;
                    a[i + j] = u + v < mod ? u + v : u + v - mod;
                    a[i + j + half] = u - v >= 0 ? u - v : u - v + mod;
                }
            }
        }

        if(invert) {
            int64_t inv_n = pw(n, mod - 2);
            for(auto& x: a) {
                x = x * inv_n % mod;
            }
        }
    }

    static vector<int64_t> mult(vector<int64_t> a, vector<int64_t> b) {
        if(a.empty() || b.empty()) {
            return {};
        }

        int res_size = (int)a.size() + (int)b.size() - 1;
        if(min((int)a.size(), (int)b.size()) <= 32) {
            vector<int64_t> res(res_size, 0);
            for(int i = 0; i < (int)a.size(); i++) {
                for(int j = 0; j < (int)b.size(); j++) {
                    res[i + j] = (res[i + j] + a[i] * b[j]) % mod;
                }
            }
            return res;
        }

        int n = 1;
        while(n < res_size) {
            n <<= 1;
        }

        a.resize(n);
        b.resize(n);
        ntt(a, false);
        ntt(b, false);
        for(int i = 0; i < n; i++) {
            a[i] = a[i] * b[i] % mod;
        }

        ntt(a, true);
        a.resize(res_size);
        return a;
    }
};

// int64_t values must be in [0, mod).
// vector<int64_t> c = NTT<>::mult(a, b);
// NTT<>::ntt(a, false); ... NTT<>::ntt(a, true);

#endif  // NTT_HPP
