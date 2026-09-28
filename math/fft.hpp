#ifndef FFT_HPP
#define FFT_HPP

#include <bits/stdc++.h>
using namespace std;

// Complex FFT for integer convolutions.
// mult: exact convolution while |result| stays below ~1e15.
// mult_mod: convolution modulo an arbitrary mod (up to ~1e9 + 7). Values are
// split into pieces of base B = ceil(mod^(1/pieces)) so every intermediate is
// exact in doubles. Default uses 2 pieces (4 FFTs): safe for random values up
// to lengths ~1e6, but adversarial values near mod - 1 can fail past ~5e5.
// high_precision = true uses 3 pieces (6 FFTs) and stays exact for lengths
// ~1e6 with values near mod - 1. Roots are computed once in long double.

class FFT {
  private:
    using cd = complex<double>;

    // rt[k + j] = exp(2 * pi * i * j / (2 * k)) for power of two k, j < k.
    inline static vector<cd> rt = {cd(1, 0), cd(1, 0)};

    // Plain product, avoids the slow NaN handling of std::complex.
    static cd mul(const cd& x, const cd& y) {
        return cd(
            x.real() * y.real() - x.imag() * y.imag(),
            x.real() * y.imag() + x.imag() * y.real()
        );
    }

    static void prepare_roots(int n) {
        if((int)rt.size() >= n) {
            return;
        }

        int k = (int)rt.size();
        rt.resize(n);
        const long double pi = acosl(-1);
        for(; k < n; k <<= 1) {
            for(int j = 0; j < k; j++) {
                long double ang = pi * j / k;
                rt[k + j] = cd((double)cosl(ang), (double)sinl(ang));
            }
        }
    }

    // In-place forward transform, size of a must be a power of two.
    static void fft(vector<cd>& a) {
        int n = (int)a.size();
        prepare_roots(n);
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

        for(int k = 1; k < n; k <<= 1) {
            for(int i = 0; i < n; i += 2 * k) {
                for(int j = 0; j < k; j++) {
                    cd z = mul(rt[j + k], a[i + j + k]);
                    a[i + j + k] = a[i + j] - z;
                    a[i + j] += z;
                }
            }
        }
    }

    static void inv_fft(vector<cd>& a) {
        int n = (int)a.size();
        reverse(a.begin() + 1, a.end());
        fft(a);
        for(auto& x: a) {
            x /= n;
        }
    }

    // Convolves a = sum a[s] * B^s with b = sum b[s] * B^s piecewise.
    // Returns c[r] = sum over s + t = r of conv(a[s], b[t]), rounded.
    static vector<vector<int64_t>> conv_pieces(
        const vector<vector<int64_t>>& a, const vector<vector<int64_t>>& b
    ) {
        int p = (int)a.size();
        int res_size = (int)a[0].size() + (int)b[0].size() - 1;
        int n = 1;
        while(n < res_size) {
            n <<= 1;
        }

        // Real inputs a[0..p), b[0..p) packed two per complex vector.
        auto real_input = [&](int id) -> const vector<int64_t>& {
            return id < p ? a[id] : b[id - p];
        };

        vector<vector<cd>> f(p, vector<cd>(n));
        for(int q = 0; q < p; q++) {
            const auto& x = real_input(2 * q);
            const auto& y = real_input(2 * q + 1);
            for(int i = 0; i < (int)x.size(); i++) {
                f[q][i].real(x[i]);
            }

            for(int i = 0; i < (int)y.size(); i++) {
                f[q][i].imag(y[i]);
            }

            fft(f[q]);
        }

        // For each pair (k, n - k) unpack spectra, multiply and repack
        // outputs c[2q] + i * c[2q + 1] into f[q].
        vector<cd> sa(p), sb(p), c(2 * p);
        for(int k = 0; k <= n / 2; k++) {
            int j = (n - k) & (n - 1);
            for(int q = 0; q < p; q++) {
                cd u = f[q][k], v = conj(f[q][j]);
                cd x = (u + v) * 0.5, y = mul(u - v, cd(0, -0.5));
                (2 * q < p ? sa[2 * q] : sb[2 * q - p]) = x;
                (2 * q + 1 < p ? sa[2 * q + 1] : sb[2 * q + 1 - p]) = y;
            }

            fill(c.begin(), c.end(), cd(0, 0));
            for(int s = 0; s < p; s++) {
                for(int t = 0; t < p; t++) {
                    c[s + t] += mul(sa[s], sb[t]);
                }
            }

            for(int q = 0; q < p; q++) {
                cd lo = c[2 * q], hi = c[2 * q + 1];
                f[q][k] = lo + cd(-hi.imag(), hi.real());
                f[q][j] = conj(lo) + cd(hi.imag(), hi.real());
            }
        }

        vector<vector<int64_t>> res(2 * p - 1, vector<int64_t>(res_size));
        for(int q = 0; q < p; q++) {
            inv_fft(f[q]);
            for(int i = 0; i < res_size; i++) {
                res[2 * q][i] = llround(f[q][i].real());
                if(2 * q + 1 < 2 * p - 1) {
                    res[2 * q + 1][i] = llround(f[q][i].imag());
                }
            }
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

        return conv_pieces({a}, {b})[0];
    }

    // Values must be in [0, mod).
    template<bool high_precision = false>
    static vector<int64_t> mult_mod(
        const vector<int64_t>& a, const vector<int64_t>& b, int64_t mod
    ) {
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

        const int p = high_precision ? 3 : 2;
        int64_t base = 1;
        while(true) {
            int64_t pw = 1;
            for(int s = 0; s < p; s++) {
                pw *= base;
            }

            if(pw >= mod) {
                break;
            }
            base++;
        }

        auto split = [&](const vector<int64_t>& x) {
            vector<vector<int64_t>> pieces(p, vector<int64_t>((int)x.size()));
            for(int i = 0; i < (int)x.size(); i++) {
                int64_t v = x[i];
                for(int s = 0; s < p; s++) {
                    pieces[s][i] = v % base;
                    v /= base;
                }
            }

            return pieces;
        };

        auto c = conv_pieces(split(a), split(b));
        vector<int64_t> res(res_size, 0);
        int64_t coef = 1;
        for(int r = 0; r < 2 * p - 1; r++) {
            for(int i = 0; i < res_size; i++) {
                res[i] = (res[i] + c[r][i] % mod * coef) % mod;
            }
            coef = coef * (base % mod) % mod;
        }

        return res;
    }
};

// vector<int64_t> c = FFT::mult(a, b);                // exact, |c[i]| < ~1e15
// vector<int64_t> c = FFT::mult_mod(a, b, mod);       // 2 pieces, 4 FFTs
// vector<int64_t> c = FFT::mult_mod<true>(a, b, mod); // 3 pieces, 6 FFTs
// For mult_mod, values must be in [0, mod), mod up to ~1e9 + 7. Use the
// high precision version for long inputs (~1e6) with large values.

#endif  // FFT_HPP
