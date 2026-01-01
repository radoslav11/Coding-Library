#include <bits/stdc++.h>
using namespace std;

template<class T = string>
class AhoCorasick {
  private:
    using G = conditional_t<
        is_same_v<T, const char*> || is_same_v<T, char*> || is_array_v<T>, char,
        typename T::value_type>;

    struct State {
        map<G, int> to;
        int link;
        int word_id;
        State() : link(0), word_id(-1) {}
    };

  public:
    vector<State> states;

    AhoCorasick() { clear(); }

    void clear() {
        states.clear();
        states.emplace_back();
    }

    void add_word(const T& word, int id = -1) {
        int u = 0;
        for(const G& c: word) {
            if(!states[u].to.count(c)) {
                states[u].to[c] = states.size();
                states.emplace_back();
            }
            u = states[u].to[c];
        }
        states[u].word_id = id;
    }

    void build() {
        queue<int> Q;
        states[0].link = -1;

        for(auto& [c, v]: states[0].to) {
            states[v].link = 0;
            Q.push(v);
        }

        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();

            for(auto& [c, v]: states[u].to) {
                int j = states[u].link;
                while(j != -1 && !states[j].to.count(c)) {
                    j = states[j].link;
                }
                states[v].link = (j == -1) ? 0 : states[j].to[c];
                Q.push(v);
            }
        }
    }

    int go(int u, const G& c) const {
        while(u != -1 && !states[u].to.count(c)) {
            u = states[u].link;
        }
        return (u == -1) ? 0 : states[u].to.at(c);
    }

    int link(int u) const { return states[u].link; }
    int word_id(int u) const { return states[u].word_id; }
    int size() const { return states.size(); }

    vector<int> traverse(const T& text) const {
        vector<int> result;
        result.reserve(text.size());
        int u = 0;
        for(const G& c: text) {
            u = go(u, c);
            result.push_back(u);
        }
        return result;
    }

    vector<vector<int>> build_link_tree() const {
        vector<vector<int>> adj(states.size());
        for(int i = 1; i < (int)states.size(); i++) {
            adj[states[i].link].push_back(i);
        }
        return adj;
    }
};
