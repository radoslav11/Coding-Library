#include <bits/stdc++.h>
using namespace std;

template<typename T, template<typename...> class Container = multiset>
class SetLazy : private Container<T> {
  private:
    T lazy_value = T();

    class Iterator {
      private:
        typename Container<T>::iterator it;
        const T& lazy_value;

      public:
        Iterator(typename Container<T>::iterator it, const T& lazy_value)
            : it(it), lazy_value(lazy_value) {}

        T operator*() const { return *it + lazy_value; }
        Iterator& operator++() {
            ++it;
            return *this;
        }
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++it;
            return tmp;
        }
        bool operator==(const Iterator& other) const { return it == other.it; }
        bool operator!=(const Iterator& other) const { return it != other.it; }

        typename Container<T>::iterator base() const { return it; }
    };

  public:
    void insert(const T& value) { Container<T>::insert(value - lazy_value); }

    void erase(const T& value) { Container<T>::erase(value - lazy_value); }
    void erase(Iterator it) { Container<T>::erase(it.base()); }

    void add_all(const T& x) { lazy_value += x; }

    T get_lazy_value() const { return lazy_value; }

    bool count(const T& value) const {
        return Container<T>::find(value - lazy_value) != Container<T>::end();
    }

    Iterator find(const T& value) {
        auto it = Container<T>::find(value - lazy_value);
        return it != Container<T>::end() ? Iterator(it, lazy_value) : end();
    }

    Iterator lower_bound(const T& value) {
        return Iterator(
            Container<T>::lower_bound(value - lazy_value), lazy_value
        );
    }

    Iterator upper_bound(const T& value) {
        return Iterator(
            Container<T>::upper_bound(value - lazy_value), lazy_value
        );
    }

    Iterator begin() { return Iterator(Container<T>::begin(), lazy_value); }
    Iterator end() { return Iterator(Container<T>::end(), lazy_value); }

    const Iterator begin() const {
        return Iterator(Container<T>::begin(), lazy_value);
    }
    const Iterator end() const {
        return Iterator(Container<T>::end(), lazy_value);
    }

    using Container<T>::size;
    using Container<T>::empty;

    void merge(const SetLazy<T, Container>& other) {
        for(const auto& value: other) {
            insert(value);
        }
    }

    void print() const {
        std::cout << "{";
        for(const auto& value: *this) {
            std::cout << value << ", ";
        }
        std::cout << "}" << std::endl;
    }
};

/*
SetLazy<int> multiset_lazy // Uses multiset by default
SetLazy<int, std::set> set_lazy; // Uses set explicitly
*/
