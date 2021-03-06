#ifndef PROGRAMMINGWITHCOMPONENTS_LIST_POOL_H
#define PROGRAMMINGWITHCOMPONENTS_LIST_POOL_H

#include <cstddef>
#include <vector>
#include <ostream>
#include "util.h"

template <typename T, typename N = std::size_t>
class list_pool {
public:
    typedef T value_type;
    typedef N list_type;

private:
    struct node_t {
        T value;
        N next;

        friend
        std::ostream& operator<<(std::ostream& os, const node_t& x) {
            return os << "node(v: " << x.value << ", n: " << x.next << ")";
        }
    };

    std::vector<node_t> pool;

    node_t& node(list_type x) {
        return pool[x - 1];
    }

    const node_t& node(list_type x) const {
        return pool[x - 1];
    }

    list_type new_list() {
        pool.push_back(node_t{});
        return pool.size();
    }

    list_type free_list;

public:
    typedef typename std::vector<node_t>::size_type size_type;

    list_type end() const {
        return list_type(0);
    }

    bool is_end(list_type x) const {
        return x == end();
    }

    size_type size() const {
        return pool.size();
    }

    void reserve(std::size_t n) {
        pool.reserve(n);
    }

    list_pool() : free_list(end()) {
        pool.reserve(24);
    }

    T& value(list_type x) {
        return node(x).value;
    }

    const T& value(list_type x) const {
        return node(x).value;
    }

    list_type& next(list_type x) {
        return node(x).next;
    }

    const list_type& next(list_type x) const {
        return node(x).next;
    }

    list_type free(list_type x) {
        list_type tail = next(x);
        next(x) = free_list;
        free_list = x;
        return tail;
    }

    list_type free(list_type front, list_type back) {
        if (is_end(front)) return end();
        list_type tail = next(back);
        next(back) = free_list;
        free_list = front;
        return tail;
    }

    list_type allocate(const T& v, list_type tail) {
        list_type list = free_list;
        if (is_end(free_list))
            list = new_list();
        else
            free_list = next(free_list);
        value(list) = v;
        next(list) = tail;
        return list;
    }

    friend
    std::ostream& operator<<(std::ostream& os, const list_pool& x) {
        return join(os, std::begin(x.pool), std::end(x.pool))
            << " - size: " << x.size() << ", free_list: " << x.free_list;
    }

    // operations on queues
    // pop_front, push_front, push_back, free,...
    typedef std::pair<list_type, list_type> pair_type;

    bool empty(const pair_type& p) { return is_end(p.first); }

    pair_type empty_queue() { return pair_type{end(), end()}; }

    pair_type push_front(const pair_type& p, const T& value) {
        list_type new_node = allocate(value, p.first);
        if (empty(p)) return pair_type(new_node, new_node);
        return pair_type(new_node, p.second);
    }

    pair_type push_back(const pair_type& p, const T& value) {
        list_type new_node = allocate(value, end());
        if (empty(p)) return pair_type (new_node, new_node);
        next(p.second) = new_node;
        return pair_type(p.first, new_node);
    }

    void free(const pair_type& p) { free(p.first, p.second); }

    // iterator
    struct iterator {
        typedef list_pool::value_type value_type;
        typedef list_pool::list_type difference_type;
        typedef std::forward_iterator_tag iterator_category;

        typedef value_type& reference;
        typedef value_type* pointer;

        list_pool* pool;
        list_pool::list_type node;

        iterator() {}
        iterator(list_pool& p, list_pool::list_type node) : pool(&p), node(node) {}
        iterator(list_pool& p) : pool(&p), node(p.end()) {}

        reference operator*() const {
            return pool->value(node);
        }

        pointer operator->() const {
            return &**this;
        }

        iterator& operator++() {
            node = pool->next(node);
            return *this;
        }

        iterator& operator++(int) {
            iterator tmp(*this);
            ++*this;
            return tmp;
        }

        friend
        bool operator==(const iterator& x, const iterator& y) {
            // assert pools are equal
            return x.node == y.node;
        }

        friend
        bool operator!=(const iterator& x, const iterator& y) {
            return !(x == y);
        }
    };
};

#endif //PROGRAMMINGWITHCOMPONENTS_LIST_POOL_H
