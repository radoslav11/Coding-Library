#include <bits/stdc++.h>
using namespace std;

// wvlt = new WaveletTree();
// wvlt->init(arr, 0, n);                     
// 
// All queries are 1-indexed.

class WaveletTree {
  private:
    int lo{1}, hi{0};
    WaveletTree *left{nullptr}, *right{nullptr};
    vector<int> b;
    int psz{0};

  public:
    WaveletTree() = default;

    void init(const vector<int>& arr, int x, int y) {
        lo = x;
        hi = y;
        if(lo == hi || arr.empty()) {
            return;
        }
        int mid = (lo + hi) >> 1;
        auto f = [mid](int x) { return x <= mid; };
        b.resize(arr.size() + 1);
        b[0] = 0;
        for(size_t i = 0; i < arr.size(); ++i) {
            b[i + 1] = b[i] + f(arr[i]);
        }
        psz = arr.size();

        // Create a mutable copy of the array
        vector<int> arr_copy(arr);
        auto pivot = stable_partition(arr_copy.begin(), arr_copy.end(), f);

        left = new WaveletTree();
        right = new WaveletTree();
        left->init(vector<int>(arr_copy.begin(), pivot), lo, mid);
        right->init(vector<int>(pivot, arr_copy.end()), mid + 1, hi);
    }

    int kth(int l, int r, int k) const {
        if(l > r) {
            return 0;
        }
        if(lo == hi) {
            return lo;
        }
        int inLeft = b[r] - b[l - 1], lb = b[l - 1], rb = b[r];
        if(k <= inLeft) {
            return left->kth(lb + 1, rb, k);
        }
        return right->kth(l - lb, r - rb, k - inLeft);
    }

    int LTE(int l, int r, int k) const {
        if(l > r || k < lo) {
            return 0;
        }
        if(hi <= k) {
            return r - l + 1;
        }
        int lb = b[l - 1], rb = b[r];
        return left->LTE(lb + 1, rb, k) + right->LTE(l - lb, r - rb, k);
    }

    int count(int l, int r, int k) const {
        if(l > r || k < lo || k > hi) {
            return 0;
        }
        if(lo == hi) {
            return r - l + 1;
        }
        int lb = b[l - 1], rb = b[r];
        int mid = (lo + hi) >> 1;
        if(k <= mid) {
            return left->count(lb + 1, rb, k);
        }
        return right->count(l - lb, r - rb, k);
    }
};
