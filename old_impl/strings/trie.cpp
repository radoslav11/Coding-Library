#include <bits/stdc++.h>

using namespace std;

struct trie {
    vector<int> cnt, len;
    vector<map<char, int>> to;

    trie() { clear(); }

    void clear() {
        cnt.assign(1, 0);
        len.assign(1, 0);
        to.assign(1, map<char, int>());
    }

    void add(string s) {
        int u = 0;
        for(char c: s) {
            if(!to[u].count(c)) {
                to[u][c] = len.size();
                len.push_back(len[u] + 1);
                cnt.push_back(0);
                to.push_back(map<char, int>());
            }
            u = to[u][c];
            cnt[u]++;
        }
    }

    void del(string s) {
        int u = 0;
        for(char c: s) {
            if(!to[u].count(c)) {
                return;
            }
            u = to[u][c];
            cnt[u]--;
        }
    }

    int count(string s) {
        int u = 0;
        for(char c: s) {
            if(!to[u].count(c)) {
                return 0;
            }
            u = to[u][c];
        }

        return cnt[u];
    }
};

trie t;

void read() {}

void solve() {}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
