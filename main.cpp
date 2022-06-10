#include <iostream>
#include <numeric>
#include <random>
#include "list_pool.h"
#include "instrumented.h"
#include "min_element1_2.h"

template <typename T, typename N>
typename list_pool<T, N>::list_type
min_element(const list_pool<T, N>& p, typename list_pool<T, N>::list_type list) {
    if (p.is_end(list)) return list;
    typename list_pool<T, N>::list_type min_element = list;
    list = p.next(list);
    while (!p.is_end(list)) {
        if (p.value(list) < p.value(min_element)) min_element = list;
        list = p.next(list);
    }
    return min_element;
}

template <typename T>
struct add_instrumented {
    instrumented<T> operator()(const instrumented<T>& a, const instrumented<T>& b) {
        return instrumented<T>{a.value + b.value};
    }
};

template <typename T>
struct add_instrumented_ref {
    instrumented<T>& operator()(instrumented<T>& a, instrumented<T>& b) {
        a.value = a.value + b.value;
        return a;
    }
};

int main() {
    typedef instrumented<std::size_t> instrumented_t;
    std::vector<instrumented_t> elements;
    for (int i=0; i < 1000; ++i)
        elements.push_back(instrumented{(std::size_t)i + 1});
    std::shuffle(std::begin(elements), std::end(elements), std::random_device{});

    instrumented_base::initialize();
    auto min1_2 = min_element1_2(std::begin(elements), std::end(elements), std::less<instrumented_t>());
    std::cout << "min1_2: (" << *min1_2.first << ", " << *min1_2.second << ")" << std::endl;
    join(std::cout, std::begin(instrumented_t::counter_names), std::end(instrumented_t::counter_names)) << std::endl;
    join(std::cout, std::begin(instrumented_t::counts), std::end(instrumented_t::counts)) << std::endl;

    return 0;
}
