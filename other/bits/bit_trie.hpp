#include <bits/stdc++.h>
using namespace std;

template<class T, int num_bits>
class bit_trie {
  public:
    struct node {
        array<int, 2> to;
        int cnt;

        node() {
            to = {-1, -1};
            cnt = 0;
        }
    };

    vector<node> trie;

    void clear() { trie = {node()}; }

    bit_trie() { clear(); }

    void add(T x, int cnt) {
        int u = 0;
        trie[0].cnt += cnt;
        for(int i = num_bits - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if(trie[u].to[bit] == -1) {
                trie[u].to[bit] = (int)trie.size();
                trie.push_back(node());
            }
            u = trie[u].to[bit];
            trie[u].cnt += cnt;
        }
    }

    T max_xor(T x) {
        int u = 0;
        T res = 0;
        for(int i = num_bits - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if(trie[u].to[bit ^ 1] != -1 && trie[trie[u].to[bit ^ 1]].cnt > 0) {
                res |= (T)1 << i;
                u = trie[u].to[bit ^ 1];
            } else {
                u = trie[u].to[bit];
            }
        }

        return res;
    }
};
