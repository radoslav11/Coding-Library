#include <bits/stdc++.h>
using namespace std;
const int MAXN = (1 << 20);

random_device rd;
mt19937_64 mt(rd());

struct node {
    int sz, prior, id, rev;
    node *par, *pp, *l, *r;
    node() {
        id = 0;
        sz = 0;
        rev = 0;
        prior = 0;
        par = nullptr;
        l = nullptr;
        r = nullptr;
        pp = nullptr;
    }
    node(int v) {
        id = v;
        sz = 1;
        rev = 0;
        prior = mt();
        l = nullptr;
        r = nullptr;
        par = nullptr;
        pp = nullptr;
    }
};

int size(node *v) { return v ? v->sz : 0; }

void push(node *&t) {
    if(!t) {
        return;
    }
    if(t->rev) {
        swap(t->l, t->r);
        if(t->l) {
            t->l->rev ^= 1;
        }
        if(t->r) {
            t->r->rev ^= 1;
        }
        t->rev = 0;
    }
}

void pull(node *&v) {
    if(!v) {
        return;
    }

    push(v->l);
    push(v->r);

    v->par = nullptr;
    v->sz = size(v->l) + size(v->r) + 1;

    if(v->l) {
        v->l->par = v;
    }
    if(v->r) {
        v->r->par = v;
    }

    if(v->l && v->l->pp) {
        v->pp = v->l->pp, v->l->pp = nullptr;
    }
    if(v->r && v->r->pp) {
        v->pp = v->r->pp, v->r->pp = nullptr;
    }
}

void merge(node *&t, node *l, node *r) {
    push(l), push(r);
    if(!l) {
        t = r;
        return;
    }
    if(!r) {
        t = l;
        return;
    }

    if(l->prior > r->prior) {
        merge(l->r, l->r, r), t = l;
    } else {
        merge(r->l, l, r->l), t = r;
    }

    pull(t);
}

void split(node *t, node *&l, node *&r, int k, int add = 0) {
    push(t);
    if(!t) {
        l = nullptr;
        r = nullptr;
        return;
    }

    int idx = add + size(t->l);
    if(idx <= k) {
        split(t->r, t->r, r, k, idx + 1), l = t;
    } else {
        split(t->l, l, t->l, k, add), r = t;
    }

    pull(t);
}

node *get_root(node *t) {
    if(!t) {
        return nullptr;
    }
    while(t->par) {
        t = t->par;
    }
    return t;
}

node *remove_right(node *t) {
    node *rt = t;

    int pos = size(rt->l);
    if(rt->rev) {
        pos = size(rt) - pos - 1;
    }
    while(rt->par) {
        if(rt->par->r == rt) {
            pos += size(rt->par->l) + 1;
        }
        if(rt->par->rev) {
            pos = size(rt->par) - pos - 1;
        }
        rt = rt->par;
    }

    node *l, *r, *pp = rt->pp;
    rt->pp = nullptr;
    split(rt, l, r, pos);

    l->pp = pp;
    if(r) {
        r->pp = t;
    }

    return l;
}

node *remove_left(node *t) {
    node *rt = t;

    int pos = size(rt->l);
    if(rt->rev) {
        pos = size(rt) - pos - 1;
    }
    while(rt->par) {
        if(rt->par->r == rt) {
            pos += size(rt->par->l) + 1;
        }
        if(rt->par->rev) {
            pos = size(rt->par) - pos - 1;
        }
        rt = rt->par;
    }

    node *l, *r, *pp = rt->pp;
    rt->pp = nullptr;
    split(rt, l, r, pos - 1);

    l->pp = pp;
    return r;
}

node *merge_trees(node *u, node *t) {
    u = get_root(u);
    t = get_root(t);
    t->pp = nullptr;
    merge(u, u, t);
    return u;
}


struct link_cut_tree {
    node *ver[MAXN];

    node *access(node *t) {
        t = remove_right(t);
        while(t->pp) {
            node *u = t->pp;
            u = remove_right(u);
            t = merge_trees(u, t);
        }

        return t;
    }

    node *find_root(node *u) {
        u = access(u);
        push(u);
        while(u->l) {
            u = u->l, push(u);
        }
        access(u);
        return u;
    }

    void make_root(node *u) {
        u = access(u);
        u->rev ^= 1;
        push(u);
    }

    void link(node *u, node *w) {
        make_root(u);
        access(w);
        merge_trees(w, u);
    }

    void cut(node *p) {
        access(p);
        remove_left(p);
    }

    int depth(node *u) {
        u = access(u);
        return size(u);
    }

    node *lca(node *u, node *v) {
        if(u == v) {
            return u;
        }
        if(depth(u) > depth(v)) {
            swap(u, v);
        }

        access(v);
        access(u);

        return get_root(v)->pp;
    }

    /// normal functions
    void init(int c) {
        for(int i = 0; i <= c; i++) {
            ver[i] = new node(i);
        }
    }
    inline int lca(int u, int v) { return lca(ver[u], ver[v])->id; }
    inline int root(int u) { return find_root(ver[u])->id; }
    inline void link(int u, int v) { link(ver[u], ver[v]); }
    inline void make_root(int u) { make_root(ver[u]); }
    inline int depth(int u) { return depth(ver[u]); }
    inline void cut(int u) { cut(ver[u]); }
};

int n, m;

void read() { cin >> n >> m; }

link_cut_tree lct;

void solve() {
    lct.init(n);

    while(m--) {
        string type;
        cin >> type;

        if(type == "add") {
            int u, w;
            cin >> u >> w;
            lct.link(u, w);
        } else if(type == "conn") {
            int u, v;
            cin >> u >> v;
            cout << (lct.root(u) == lct.root(v) ? "YES" : "NO") << endl;
        } else if(type == "rem") {
            int u, v;
            cin >> u >> v;
            if(lct.depth(u) > lct.depth(v)) {
                swap(u, v);
            }
            lct.cut(v);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    read();
    solve();
    return 0;
}
