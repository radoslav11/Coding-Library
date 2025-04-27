#include <bits/stdc++.h>
using namespace std;

template<class T, bool (*cmp)(T, T)>
class Heap {
  private:
    vector<T> heap_values;
    vector<int> heap, pos_in_heap;

    void push_up(int id) {
        while(id > 0 &&
              cmp(heap_values[heap[id]], heap_values[heap[(id - 1) / 2]])) {
            swap(heap[id], heap[(id - 1) / 2]);
            swap(pos_in_heap[heap[id]], pos_in_heap[heap[(id - 1) / 2]]);
            id = (id - 1) / 2;
        }
    }

    void push_down(int id) {
        while(2 * id + 1 < heap.size()) {
            int child = 2 * id + 1;
            if(child + 1 < heap.size() &&
               cmp(heap_values[heap[child + 1]], heap_values[heap[child]])) {
                child++;
            }
            if(cmp(heap_values[heap[id]], heap_values[heap[child]])) {
                break;
            }
            swap(heap[id], heap[child]);
            swap(pos_in_heap[heap[id]], pos_in_heap[heap[child]]);
            id = child;
        }
    }

  public:
    Heap() { clear(); }

    void clear() {
        heap.clear();
        heap_values.clear();
        pos_in_heap.clear();
    }

    int push(T val) {
        heap.push_back(heap_values.size());
        pos_in_heap.push_back(heap.size() - 1);
        heap_values.push_back(val);
        push_up(heap.size() - 1);
        return heap_values.size() - 1;
    }

    T pop() {
        int ret_node = heap[0];
        swap(pos_in_heap[ret_node], pos_in_heap[heap.back()]);
        swap(heap[0], heap[heap.size() - 1]);
        heap.pop_back();
        pos_in_heap[ret_node] = -1;
        if(heap.size() > 0) {
            push_down(0);
        }
        return heap_values[ret_node];
    }

    size_t size() { return heap.size(); }
    bool empty() { return heap.size() == 0; }

    T top() { return heap_values[heap[0]]; }

    int top_node() { return heap[0]; }

    void update(int node, T val) {
        int p = pos_in_heap[node];
        bool is_push_down = cmp(heap_values[node], val);

        if(is_push_down) {
            heap_values[node] = val;
            push_down(p);
        } else {
            heap_values[node] = val;
            push_up(p);
        }
    }
};

