#include <bits/stdc++.h>
using namespace std;

template<class T = int>
class MeldableHeap {
  private:
    static uint32_t rng() {
        static mt19937 static_rng(random_device{}());
        return static_rng();
    }

    struct Node {
        T key;
        Node *left, *right;

        Node(T _key) : key(_key), left(nullptr), right(nullptr) {}
    };

    Node* merge(Node* a, Node* b) {
        if(!a) {
            return b;
        }
        if(!b) {
            return a;
        }

        if(a->key > b->key) {
            swap(a, b);
        }

        Node* q = new Node(a->key);
        if(rng() & 1) {
            q->left = merge(a->left, b);
            q->right = a->right;
        } else {
            q->left = a->left;
            q->right = merge(a->right, b);
        }

        return q;
    }

    pair<Node*, Node*> pop(Node* a) {
        Node* head = new Node(a->key);
        Node* tail = merge(a->left, a->right);
        return {head, tail};
    }

  public:
    Node* root;

    MeldableHeap() : root(nullptr) {}
    MeldableHeap(Node* _root) : root(_root) {}

    MeldableHeap copy() const {
        MeldableHeap new_heap;
        new_heap.root = root;
        return new_heap;
    }

    MeldableHeap merge(const MeldableHeap<T>& other) {
        MeldableHeap new_heap;
        new_heap.root = merge(root, other.root);
        return new_heap;
    }

    friend MeldableHeap merge(
        const MeldableHeap<T>& a, const MeldableHeap<T>& b
    ) {
        return a.merge(b);
    }

    void push(T key) {
        Node* new_node = new Node(key);
        root = merge(root, new_node);
    }

    T pop() {
        assert(root);
        auto [head, tail] = pop(root);
        root = tail;
        return head->key;
    }

    T top() const { return root->key; }

    tuple<T, MeldableHeap<T>, MeldableHeap<T>> trio() const {
        return {
            root->key, MeldableHeap<T>{root->left}, MeldableHeap<T>{root->right}
        };
    }

    bool empty() const { return root == nullptr; }

    bool operator<(const MeldableHeap<T>& other) const {
        return top() < other.top();
    }
};
