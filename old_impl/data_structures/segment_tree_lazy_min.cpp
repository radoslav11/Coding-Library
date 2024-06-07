#include <bits/stdc++.h>
#include <limits>
using namespace std;

template<class T>
class segment_tree_min {
  private:
    static const T max_possible_value = std::numeric_limits<T>::max();

    struct node {
        T mn;
        int pos;

        node() {
            mn = max_possible_value;
            pos = -1;
        }

        node(T val, int p) {
            mn = val;
            pos = p;
        }
    };

    node merge(node l, node r) {
        node temp;
        temp.mn = min(l.mn, r.mn);
        if(l.mn == temp.mn) {
            temp.pos = l.pos;
        } else {
            temp.pos = r.pos;
        }

        return temp;
    }

    void push(int l, int r, int idx) {
        if(lazy[idx]) {
            tr[idx].mn += lazy[idx];

            if(l != r) {
                lazy[2 * idx + 1] += lazy[idx];
                lazy[2 * idx + 2] += lazy[idx];
            }

            lazy[idx] = 0;
        }
    }

    int n;
    vector<T> lazy;
    vector<node> tr;

    void init(int l, int r, int idx, const vector<T> &a) {
        if(l == r) {
            tr[idx] = node(a[l], l);
            return;
        }

        int mid = (l + r) >> 1;
        init(l, mid, 2 * idx + 1, a);
        init(mid + 1, r, 2 * idx + 2, a);

        tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
    }

    void update(int qL, int qR, T val, int l, int r, int idx) {
        push(l, r, idx);

        if(qL > r || l > qR) {
            return;
        }

        if(qL <= l && r <= qR) {
            lazy[idx] += val;
            push(l, r, idx);
            return;
        }

        int mid = (l + r) >> 1;
        update(qL, qR, val, l, mid, 2 * idx + 1);
        update(qL, qR, val, mid + 1, r, 2 * idx + 2);

        tr[idx] = merge(tr[2 * idx + 1], tr[2 * idx + 2]);
    }

    node query(int qL, int qR, int l, int r, int idx) {
        push(l, r, idx);

        if(l > qR || r < qL) {
            return node();
        }

        if(qL <= l && r <= qR) {
            return tr[idx];
        }

        int mid = (l + r) >> 1;
        return merge(
            query(qL, qR, l, mid, 2 * idx + 1),
            query(qL, qR, mid + 1, r, 2 * idx + 2)
        );
    }

  public:
    void init(const vector<T> &a) {
        n = a.size();
        tr.resize(4 * n);
        lazy.assign(4 * n, 0);
        init(0, n - 1, 0, a);
    }

    void update(int qL, int qR, int val) {
        if(qR < qL) {
            return;
        }
        update(qL, qR, val, 0, n - 1, 0);
    }

    node query(int qL, int qR) {
        if(qR < qL) {
            return node();
        }

        return query(qL, qR, 0, n - 1, 0);
    }
};

int n, m;
vector<int> a;

void read() {
    cin >> n >> m;
    a.resize(n);
    for(int i = 0; i < n; i++) {
        cin >> a[i];
    }
}

segment_tree_min<int> t;

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
