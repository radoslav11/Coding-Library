#include <bits/stdc++.h>
using namespace std;

// Mine was slow, so based it on maspy's implementation:
// https://codeforces.com/contest/2097/submission/317301108

template<
    class T, T (*merge)(T, T), T (*e)(), T (*inverse)(T), class G,
    T (*lazy_apply)(G, T), G (*lazy_merge)(G, G), G (*lazy_init)(T)>
class LinkCutTree {
  private:
    const G lazy_e = lazy_init(e());

    struct Node {
        Node *left, *right, *par;
        T value;        // Node value
        T path_agg;     // Path aggregate
        T subtree_agg;  // Subtree aggregate
        G lazy;         // Lazy propagation value
        bool flip;      // Reverse tag
        int idx;        // Node index

        Node(T val = e(), int id = -1)
            : left(nullptr),
              right(nullptr),
              par(nullptr),
              value(val),
              path_agg(val),
              lazy(lazy_init(e())),
              flip(false),
              idx(id) {}
    };

    vector<Node> nodes;

    void push(Node* x) {
        if(!x) {
            return;
        }
        if(x->flip) {
            swap(x->left, x->right);
            if(x->left) {
                x->left->flip = !x->left->flip;
            }
            if(x->right) {
                x->right->flip = !x->right->flip;
            }
            x->flip = false;
        }
        if(x->lazy != lazy_e) {
            x->value = lazy_apply(x->lazy, x->value);
            x->path_agg = lazy_apply(x->lazy, x->path_agg);
            if(x->left) {
                x->left->lazy = lazy_merge(x->left->lazy, x->lazy);
            }
            if(x->right) {
                x->right->lazy = lazy_merge(x->right->lazy, x->lazy);
            }
            x->lazy = lazy_e;
        }
    }

    void pull(Node* x) {
        if(!x) {
            return;
        }
        push(x->left);
        push(x->right);
        x->path_agg = e();
        if(x->left) {
            x->path_agg = merge(x->left->path_agg, x->path_agg);
        }
        x->path_agg = merge(x->path_agg, x->value);
        if(x->right) {
            x->path_agg = merge(x->path_agg, x->right->path_agg);
        }
    }

    inline int state(Node* n) {
        if(!n->par) {
            return 0;
        }
        if(n->par->left == n) {
            return 1;
        }
        if(n->par->right == n) {
            return -1;
        }
        return 0;
    }

    bool is_root(Node* x) { return state(x) == 0; }

    void rotate(Node* x) {
        Node *p = x->par, *pp = p->par;
        if(p->left == x) {
            p->left = x->right;
            if(x->right) {
                x->right->par = p;
            }
            x->right = p;
        } else {
            p->right = x->left;
            if(x->left) {
                x->left->par = p;
            }
            x->left = p;
        }
        p->par = x;
        x->par = pp;
        if(pp) {
            if(pp->left == p) {
                pp->left = x;
            } else if(pp->right == p) {
                pp->right = x;
            } else {
                change_light(pp, p, x);
            }
        }
        pull(p);
        pull(x);
    }

    void splay(Node* x) {
        push(x);
        while(!is_root(x)) {
            Node *p = x->par, *pp = p->par;
            if(is_root(p)) {
                push(p);
                push(x);
                rotate(x);
            } else {
                push(pp);
                push(p);
                push(x);
                if(state(x) == state(p)) {
                    rotate(p);
                    rotate(x);
                } else {
                    rotate(x);
                    rotate(x);
                }
            }
        }
    }

    Node* expose(Node* x) {
        Node* rp = nullptr;
        for(Node* c = x; c; c = c->par) {
            splay(c);
            if(c->right) {
                add_light(c, c->right);
            }
            c->right = rp;
            if(rp) {
                erase_light(c, rp);
            }
            pull(c);
            rp = c;
        }
        splay(x);
        return rp;
    }

    void apply_lazy_to_node(Node* node, const G& lazy_val) {
        if(!node) {
            return;
        }
        node->lazy = lazy_merge(node->lazy, lazy_val);
        push(node);
    }

    void add_light(Node* c, Node* light) {
        c->subtree_agg = merge(c->subtree_agg, light->path_agg);
    }

    void erase_light(Node* c, Node* light) {
        c->subtree_agg = merge(c->subtree_agg, inverse(light->path_agg));
    }

  public:
    LinkCutTree(int n) : nodes(n) {
        for(int i = 0; i < n; i++) {
            nodes[i] = Node(e(), i);
        }
    }

    LinkCutTree(const vector<T>& values) : nodes(values.size()) {
        for(int i = 0; i < values.size(); i++) {
            nodes[i] = Node(values[i], i);
        }
    }

    Node* access(int x) {
        Node* u = &nodes[x];
        expose(u);
        return u;
    }

    void reroot(int x) {
        access(x);
        nodes[x].flip ^= true;
        push(&nodes[x]);
    }

    void link(int u, int v) {
        reroot(u);
        access(v);
        assert(!nodes[u].par && !nodes[v].right);
        nodes[u].par = &nodes[v];
        nodes[v].right = &nodes[u];
        pull(&nodes[v]);
    }

    void cut(int u, int v) {
        reroot(u);
        access(v);
        assert(!nodes[v].par && nodes[v].left == &nodes[u]);
        nodes[v].left->par = nullptr;
        nodes[v].left = nullptr;
        pull(&nodes[v]);
    }

    bool connected(int u, int v) {
        if(u == v) {
            return true;
        }
        access(u);
        access(v);
        return nodes[u].par != nullptr;
    }

    int lca(int u, int v) {
        if(u == v) {
            return u;
        }
        access(u);
        Node* ancestor = expose(&nodes[v]);
        return nodes[u].par ? ancestor->idx : -1;
    }

    T path_query(int u, int v) {
        reroot(u);
        access(v);
        return nodes[v].path_agg;
    }

    void update(int u, const T& val) {
        access(u);
        nodes[u].value = val;
        pull(&nodes[u]);
    }

    void path_update(int u, int v, const G& lazy_val) {
        reroot(u);
        access(v);
        apply_lazy_to_node(&nodes[v], lazy_val);
    }

    T get(int u) {
        access(u);
        return nodes[u].value;
    }

    int get_root(int u) {
        access(u);
        Node* c = &nodes[u];
        push(c);
        while(c->left) {
            c = c->left;
            push(c);
        }
        splay(c);
        return c->idx;
    }

    int jump(int u, int v, int k) {
        reroot(v);
        access(u);
        assert(0 <= k && k < nodes[u].size);
        Node* c = &nodes[u];
        while(true) {
            push(c);
            int rs = c->right ? c->right->size : 0;
            if(k < rs) {
                c = c->right;
                continue;
            }
            if(k == rs) {
                break;
            }
            k -= (rs + 1);
            c = c->left;
        }
        splay(c);
        return c->idx;
    }

    Node* get_node(int idx) { return &nodes[idx]; }

    int size() const { return nodes.size(); }

    // Overload this for more complicated updates:
    void change_light(Node* pp, Node* p, Node* x) {
        // noop by default.
    }

    // Useful for debugging.
    void print_node(Node* node, int indent = 0) {
        cout << string(indent, ' ') << string(16, '-') << '\n';
        cout << string(indent, ' ') << "Node " << node->idx << ": ";
        cout << string(indent, ' ') << "Value: " << node->value << '\n';
        cout << string(indent, ' ') << "Path agg: " << node->path_agg << '\n';
        cout << string(indent, ' ') << "Subtree agg: " << node->subtree_agg
             << '\n';
        cout << string(indent, ' ') << "Virtual agg: " << node->virtual_agg
             << '\n';
        cout << string(indent, ' ') << "Lazy: " << node->lazy.add_val << '\n';
        cout << string(indent, ' ') << "HAS PARENT: "
             << (node->par ? to_string(node->par->idx) : "null") << '\n';
        cout << string(indent, ' ') << string(16, '-') << '\n';
    }

    void print_tree(Node* node, int indent = 0) {
        print_node(node, indent);
        push(node);
        if(node->ch[0]) {
            cout << string(indent, ' ') << "Left child:\n";
            print_tree(node->ch[0], indent + 2);
        }
        if(node->ch[1]) {
            cout << string(indent, ' ') << "Right child:\n";
            print_tree(node->ch[1], indent + 2);
        }
        pull(node);
    }
};
