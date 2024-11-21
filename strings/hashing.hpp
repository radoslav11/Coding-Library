#include <bits/stdc++.h>
using namespace std;

class HashMeta {
  private:
    void set_random_base() {
        seed_seq seed{
            (uint32_t)chrono::duration_cast<chrono::nanoseconds>(
                chrono::high_resolution_clock::now().time_since_epoch()
            )
                .count(),
            (uint32_t)random_device()(), (uint32_t)42
        };
        mt19937 rng(seed);
        base = uniform_int_distribution<uint64_t>(0, mod - 1)(rng);
    }

    void precompute_base_pow(size_t n) {
        base_pow.resize(n);
        base_pow[0] = 1;
        for(size_t i = 1; i < n; i++) {
            base_pow[i] = mul(base_pow[i - 1], base);
        }
    }

    static constexpr uint64_t add(uint64_t a, uint64_t b) {
        a += b + 1;
        a = (a & mod) + (a >> 61);
        return a - 1;
    }

    static constexpr uint64_t sub(uint64_t a, uint64_t b) {
        return add(a, mod - b);
    }

    static constexpr uint64_t mul(uint64_t a, uint64_t b) {
        uint64_t l1 = (uint32_t)a, h1 = a >> 32, l2 = (uint32_t)b, h2 = b >> 32;
        uint64_t l = l1 * l2, m = l1 * h2 + l2 * h1, h = h1 * h2;
        uint64_t ret =
            (l & mod) + (l >> 61) + (h << 3) + (m >> 29) + (m << 35 >> 3) + 1;
        ret = (ret & mod) + (ret >> 61);
        ret = (ret & mod) + (ret >> 61);
        return ret - 1;
    }

  public:
    class hash_t {
        uint64_t h;

      public:
        hash_t() : h(0) {}
        hash_t(uint64_t h) : h(h) {}
        operator uint64_t() const { return h; }

        hash_t& operator+=(const hash_t& other) {
            h = add(h, other.h);
            return *this;
        }

        hash_t& operator-=(const hash_t& other) {
            h = sub(h, other.h);
            return *this;
        }

        hash_t& operator*=(const hash_t& other) {
            h = mul(h, other.h);
            return *this;
        }

        hash_t operator+(const hash_t& other) const {
            return hash_t(*this) += other;
        }
        hash_t operator-(const hash_t& other) const {
            return hash_t(*this) -= other;
        }
        hash_t operator*(const hash_t& other) const {
            return hash_t(*this) *= other;
        }

        bool operator==(const hash_t& other) const { return h == other.h; }
        bool operator!=(const hash_t& other) const { return h != other.h; }

        // For use in std::map and std::set
        bool operator<(const hash_t& other) const { return h < other.h; }
    };

    uint64_t base;
    vector<hash_t> base_pow;
    static constexpr uint64_t mod = (1ull << 61) - 1;

    void init(size_t n) {
        set_random_base();
        precompute_base_pow(n);
    }

    template<typename T>
    vector<hash_t> rabin_karp(const T& container) {
        vector<hash_t> h(container.size());
        for(size_t i = 0; i < container.size(); i++) {
            h[i] = (i ? h[i - 1] : hash_t(0)) * hash_t(base) +
                   hash_t(container[i]);
        }
        return h;
    }

    hash_t hash_range(int l, int r, const vector<hash_t>& h) {
        if(l == 0) {
            return h[r];
        }
        return h[r] - h[l - 1] * base_pow[r - l + 1];
    }
};

// HashMeta hash_meta;
// using hash_t = HashMeta::hash_t;
