#include <bits/stdc++.h>
using namespace std;

template<class T = string>
class SuffixAutomaton {
  private:
    using G = conditional_t<
        is_same_v<T, const char*> || is_same_v<T, char*> || is_array_v<T>, char,
        typename T::value_type>;

    struct State {
        map<G, int> to;
        int len;
        int link;
        State(int l = 0, int lnk = -1) : len(l), link(lnk) {}
    };

    int check_replace_with_clone(int p, G c) {
        int q = states[p].to[c];
        if(states[p].len + 1 == states[q].len) {
            return q;
        }

        int clone = states.size();
        states.emplace_back(states[p].len + 1, states[q].link);
        states[clone].to = states[q].to;
        while(p >= 0 && states[p].to[c] == q) {
            states[p].to[c] = clone;
            p = states[p].link;
        }
        states[q].link = clone;
        return clone;
    }

  public:
    int last;
    vector<State> states;

    SuffixAutomaton() : last(0) { clear(); }
    SuffixAutomaton(const T& s) { init(s); }

    void add_letter(G c) {
        if(states[last].to.count(c)) {
            int clone = check_replace_with_clone(last, c);
            last = clone;
            return;
        }

        int p = last;
        last = states.size();
        states.emplace_back(states[p].len + 1);

        while(p >= 0 && !states[p].to.count(c)) {
            states[p].to[c] = last;
            p = states[p].link;
        }

        if(p == -1) {
            states[last].link = 0;
            return;
        }

        int q_or_clone = check_replace_with_clone(p, c);
        states[last].link = q_or_clone;
    }

    void add_string(const T& s) {
        last = 0;
        for(char c: s) {
            add_letter(c);
        }
    }

    void clear() {
        states.clear();
        states.emplace_back();
        last = 0;
    }

    void init(const T& s) {
        clear();
        add_string(s);
    }

    vector<vector<int>> build_suffix_link_tree() {
        vector<vector<int>> adj(states.size());
        for(int i = 1; i < (int)states.size(); i++) {
            adj[states[i].link].push_back(i);
        }
        return adj;
    }
};
