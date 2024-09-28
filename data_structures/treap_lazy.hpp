#include <bits/stdc++.h>
using namespace std;

template<
    class KeyT, class T, T (*merge_func)(T, T), class LazyT, uint64_t (*rng)()>
struct TreapNode {
    KeyT key;
    T data, subtree;
    uint64_t prior;
    size_t size;
    TreapNode *left, *right;
    LazyT lazy;

    TreapNode(KeyT key, T data)
        : key(key), data(data), left(nullptr), right(nullptr), size(1) {
        prior = rng();
    }

    void pull() {
        subtree = data;
        size = 1;
        if(left) {
            left->push();
            subtree = merge_func(left->subtree, subtree);
            size += left->size;
        }
        if(right) {
            right->push();
            subtree = merge_func(subtree, right->subtree);
            size += right->size;
        }
    }

    void push() { lazy.apply_lazy(this); }

    friend void push_lazy(TreapNode* t) {
        if(t) {
            t->push();
        }
    }

    friend pair<TreapNode*, TreapNode*> split(TreapNode* t, KeyT key) {
        if(!t) {
            return {nullptr, nullptr};
        }

        t->push();
        if(key < t->key) {
            auto [left, t_left] = split(t->left, key);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] = split(t->right, key);
            t->right = t_right;
            t->pull();
            return {t, right};
        }
    }

    friend pair<TreapNode*, TreapNode*> split_by_size(
        TreapNode* t, size_t size
    ) {
        if(!t) {
            return {nullptr, nullptr};
        }

        t->push();
        if(t->left && t->left->size >= size) {
            auto [left, t_left] = split_by_size(t->left, size);
            t->left = t_left;
            t->pull();
            return {left, t};
        } else {
            auto [t_right, right] = split_by_size(
                t->right, size - 1 - (t->left ? t->left->size : 0)
            );
            t->right = t_right;
            t->pull();
            return {t, right};
        }
    }

    friend TreapNode* merge(TreapNode* l, TreapNode* r) {
        push_lazy(l);
        push_lazy(r);
        if(!l || !r) {
            return l ? l : r;
        } else if(l->prior > r->prior) {
            l->right = merge(l->right, r);
            l->pull();
            return l;
        } else {
            r->left = merge(l, r->left);
            r->pull();
            return r;
        }
    }

    friend TreapNode* unordered_merge(TreapNode* l, TreapNode* r) {
        push_lazy(l);
        push_lazy(r);
        if(!l) {
            return r;
        }
        if(!r) {
            return l;
        }
        if(l->prior < r->prior) {
            swap(l, r);
        }
        auto [t1, t2] = split(r, l->key);
        l->left = unordered_merge(l->left, t1);
        l->right = unordered_merge(l->right, t2);
        l->pull();
        return l;
    }

    friend void insert_in(TreapNode*& t, TreapNode* it) {
        if(!t) {
            t = it;
        } else {
            t->push();
            if(it->prior > t->prior) {
                auto [t1, t2] = split(t, it->key);
                it->left = t1;
                it->right = t2;
                t = it;
            } else {
                insert_in(it->key < t->key ? t->left : t->right, it);
            }
        }
        t->pull();
    }

    friend TreapNode* erase_from(
        TreapNode*& t, KeyT key, bool delete_node = false
    ) {
        t->push();
        T return_data;
        if(t->key == key) {
            auto tmp = t;
            t = merge(t->left, t->right);

            return_data = tmp->data;
            if(delete_node) {
                delete tmp;
            }
        } else {
            return_data =
                erase_from(key < t->key ? t->left : t->right, key, delete_node);
        }
        if(t) {
            t->pull();
        }
        return return_data;
    }
};

template<class KeyT, class T, T (*merge_func)(T, T), class LazyT>
class Treap {
  public:
    static uint64_t rng() {
        static mt19937_64 static_rng(random_device{}());
        return static_rng();
    }

    using Node = TreapNode<KeyT, T, merge_func, LazyT, Treap::rng>;

    void _pull_all(Node* t) {
        if(t) {
            t->push();
            _pull_all(t->left);
            _pull_all(t->right);
            t->pull();
        }
    }

    Node* root;

    Treap() { root = nullptr; }
    Treap(const vector<pair<KeyT, T>>& a) { build_cartesian_tree(a); }

    void build_cartesian_tree(const vector<pair<KeyT, T>>& a) {
        vector<Node*> st;

        function<Node*(Node*)> recycle_stack = [&](Node* last) {
            Node* new_last = st.back();
            st.pop_back();
            new_last->right = last;
            return new_last;
        };

        for(const auto& [key, val]: a) {
            Node* new_node = new Node(key, val);
            Node* last = nullptr;
            while(!st.empty() && st.back()->prior < new_node->prior) {
                last = recycle_stack(last);
            }

            new_node->left = last;
            st.push_back(new_node);
        }

        root = nullptr;
        while(!st.empty()) {
            root = recycle_stack(root);
        }

        _pull_all(root);
    }

    void insert(KeyT key, T data) {
        Node* new_node = new Node(key, data);
        insert_in(root, new_node);
    }

    void erase(KeyT key) { return erase_from(root, key); }

    friend Treap<KeyT, T, merge_func, LazyT> merge_treaps(
        Treap<KeyT, T, merge_func, LazyT> l, Treap<KeyT, T, merge_func, LazyT> r
    ) {
        Treap<KeyT, T, merge_func, LazyT> res;
        res.root = unordered_merge(l.root, r.root);
        return res;
    }
};

template<class T>
struct AddLazy {
    T add_key = 0;
    T add_data = 0;

    template<class G, uint64_t (*rng)(), T (*merge_func)(T, T)>
    void apply_lazy(TreapNode<pair<T, G>, T, merge_func, AddLazy, rng>* node) {
        if(!node || (add_key == 0 && add_data == 0)) {
            return;
        }

        node->key.first += add_key;
        node->data += add_data;
        node->subtree += add_data * (T)node->size;

        if(node->left) {
            node->left->lazy.add_data += add_data;
            node->left->lazy.add_key += add_key;
        }

        if(node->right) {
            node->right->lazy.add_data += add_data;
            node->right->lazy.add_key += add_key;
        }

        add_key = 0;
        add_data = 0;
    }
};

// int64_t add(int64_t a, int64_t b) { return a + b; }
// using TreapWithLazy = Treap<pair<int64_t, int>, int64_t, add,
// AddLazy<int64_t>>; using Node = TreapWithLazy::Node;

// Tested on https://codeforces.com/contest/702/problem/F.
// https://codeforces.com/contest/702/submission/283296920
