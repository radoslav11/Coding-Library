#ifndef FFT_HPP
#define FFT_HPP

#include <bits/stdc++.h>
using namespace std;

// Complex FFT for integer convolutions.
// mult: exact convolution while |result| stays below ~1e15.
// mult_mod: convolution modulo an arbitrary mod < 2^31. Values are taken in
// (-mod / 2, mod / 2] and split into p balanced digits of base B ~ mod^(1 / p),
// so every digit is at most ~B / 2 in absolute value.
// Default p = 2 (4 FFTs): max rounding error ~0.13 on Library Checker
// fft_killer tests and ~0.25 on adversarial inputs with n = m = 2^20 for mod
// ~1e9 (~0.4 for mod ~2^31). high_precision = true uses p = 3 (6 FFTs), with
// error < 1e-3 on all of these.
// The forward pass is decimation in frequency (output in bit-reversed order)
// and the backward pass decimation in time, so no bit reversal is needed.
// Roots are precomputed once and are accurate to ~1 ulp.

class FFT {
  private:
    using cd = complex<double>;

    // rt[k + j] = exp(i * pi * j / k) for power of two k and j < k.
    inline static vector<cd> rt = {cd(1, 0), cd(1, 0)};

    // Plain product, avoids the slow NaN handling of std::complex.
    static cd mul(const cd& x, const cd& y) {
        return cd(
            x.real() * y.real() - x.imag() * y.imag(),
            x.real() * y.imag() + x.imag() * y.real()
        );
    }

    // Each root is a product of two long double factors from tables of size
    // ~sqrt(k), so it is accurate to ~1 ulp and cheap to build.
    static void prepare_roots(int n) {
        const long double pi = acosl(-1);
        for(int k = (int)rt.size(); k < n; k <<= 1) {
            int s = 1;
            while(s * s < k) {
                s <<= 1;
            }

            vector<complex<long double>> hi(k / s), lo(s);
            for(int j = 0; j < k / s; j++) {
                hi[j] = polar(1.0L, pi * j * s / k);
            }

            for(int j = 0; j < s; j++) {
                lo[j] = polar(1.0L, pi * j / k);
            }

            rt.resize(2 * k);
            for(int j = 0; j < k; j++) {
                rt[k + j] = cd(hi[j / s] * lo[j % s]);
            }
        }
    }

    // Natural order in, transform in bit-reversed order out.
    static void dif(vector<cd>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = n >> 1; k > 0; k >>= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = i; j < i + k; j++) {
                    cd u = a[j], v = a[j + k];
                    a[j] = u + v;
                    a[j + k] = mul(u - v, rt[k + j - i]);
                }
            }
        }
    }

    // Bit-reversed order in, natural order out. dit(dif(a)) is n * a read at
    // -i, so it also serves as the inverse.
    static void dit(vector<cd>& a) {
        int n = (int)a.size();
        prepare_roots(n);
        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = i; j < i + k; j++) {
                    cd u = a[j], v = mul(a[j + k], rt[k + j - i]);
                    a[j] = u + v;
                    a[j + k] = u - v;
                }
            }
        }
    }

    // In bit-reversed order, the position holding frequency -f for the
    // position i holding f (mirror inside [2^t, 2^(t + 1))).
    static int partner(int i) { return i < 2 ? i : i ^ ((1 << __lg(i)) - 1); }

    // Splits x taken in (-mod / 2, mod / 2] into p balanced digits of base B,
    // x = sum d[s] * B^s, and stores digit s as real input number id + s.
    // Real input number id goes to f[id / 2], real part if id is even.
    static void put_digits(
        vector<vector<cd>>& f, const vector<int64_t>& a, int id, int p,
        int64_t base, int64_t mod
    ) {
        for(int i = 0; i < (int)a.size(); i++) {
            int64_t v = mod && a[i] > mod / 2 ? a[i] - mod : a[i];
            for(int s = 0; s < p; s++) {
                int64_t q = s + 1 < p ? llround((double)v / base) : 0;
                if((id + s) & 1) {
                    f[(id + s) / 2][i].imag(v - q * base);
                } else {
                    f[(id + s) / 2][i].real(v - q * base);
                }
                v = q;
            }
        }
    }

    // Convolution via p digits per value (mod = 0: p = 1, no reduction).
    // The 2p real digit sequences are packed two per complex vector, and so
    // are the 2p - 1 digit convolutions c[r] = sum over s + t = r of
    // conv(a[s], b[t]). Result is sum c[r] * B^r.
    static vector<int64_t> conv(
        const vector<int64_t>& a, const vector<int64_t>& b, int p, int64_t mod
    ) {
        int res_size = (int)a.size() + (int)b.size() - 1, n = 1;
        while(n < res_size) {
            n <<= 1;
        }

        int64_t base = mod ? ceil(pow((long double)mod, 1.0L / p)) : 1;
        vector<vector<cd>> f(p, vector<cd>(n));
        put_digits(f, a, 0, p, base, mod);
        put_digits(f, b, p, p, base, mod);
        for(auto& v: f) {
            dif(v);
        }

        // Unpack the digit spectra at i and -i, multiply, repack
        // c[2q] + i * c[2q + 1] into f[q].
        vector<cd> sp(2 * p), c(2 * p);
        for(int i = 0; i < n; i++) {
            int j = partner(i);
            if(j < i) {
                continue;
            }

            for(int id = 0; id < 2 * p; id++) {
                cd x = f[id / 2][i], y = conj(f[id / 2][j]);
                sp[id] = id & 1 ? mul(x - y, cd(0, -0.5)) : (x + y) * 0.5;
            }

            fill(c.begin(), c.end(), cd(0, 0));
            for(int s = 0; s < p; s++) {
                for(int t = 0; t < p; t++) {
                    c[s + t] += mul(sp[s], sp[p + t]);
                }
            }

            for(int q = 0; q < p; q++) {
                cd lo = c[2 * q] / (double)n, hi = c[2 * q + 1] / (double)n;
                f[q][i] = lo + cd(-hi.imag(), hi.real());
                f[q][j] = conj(lo) + cd(hi.imag(), hi.real());
            }
        }

        for(auto& v: f) {
            dit(v);
        }

        vector<int64_t> res(res_size);
        for(int i = 0; i < res_size; i++) {
            int k = (n - i) & (n - 1);
            int64_t val = 0;
            for(int r = 2 * p - 2; r >= 0; r--) {
                cd z = f[r / 2][k];
                int64_t cr = llround(r & 1 ? z.imag() : z.real());
                val = mod ? (val * base + cr) % mod : cr;
            }
            res[i] = mod && val < 0 ? val + mod : val;
        }
        return res;
    }

  public:
    static vector<int64_t> mult(
        const vector<int64_t>& a, const vector<int64_t>& b
    ) {
        if(a.empty() || b.empty()) {
            return {};
        }

        if(min((int)a.size(), (int)b.size()) <= 32) {
            vector<int64_t> res((int)a.size() + (int)b.size() - 1, 0);
            for(int i = 0; i < (int)a.size(); i++) {
                for(int j = 0; j < (int)b.size(); j++) {
                    res[i + j] += a[i] * b[j];
                }
            }
            return res;
        }

        return conv(a, b, 1, 0);
    }

    // Values must be in [0, mod).
    template<bool high_precision = false>
    static vector<int64_t> mult_mod(
        const vector<int64_t>& a, const vector<int64_t>& b, int64_t mod
    ) {
        if(a.empty() || b.empty()) {
            return {};
        }

        if(min((int)a.size(), (int)b.size()) <= 32) {
            vector<int64_t> res((int)a.size() + (int)b.size() - 1, 0);
            for(int i = 0; i < (int)a.size(); i++) {
                for(int j = 0; j < (int)b.size(); j++) {
                    res[i + j] = (res[i + j] + a[i] * b[j]) % mod;
                }
            }
            return res;
        }

        return conv(a, b, high_precision ? 3 : 2, mod);
    }
};

// vector<int64_t> c = FFT::mult(a, b);                // exact, |c[i]| < ~1e15
// vector<int64_t> c = FFT::mult_mod(a, b, mod);       // 2 digits, 4 FFTs
// vector<int64_t> c = FFT::mult_mod<true>(a, b, mod); // 3 digits, 6 FFTs
// For mult_mod, values must be in [0, mod), mod < 2^31. For an exact
// arbitrary mod convolution without doubles see NTTAnyMod in ntt.hpp.

#endif  // FFT_HPP
