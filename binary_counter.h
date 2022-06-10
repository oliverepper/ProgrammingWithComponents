#ifndef PROGRAMMINGWITHCOMPONENTS_BINARY_COUNTER_H
#define PROGRAMMINGWITHCOMPONENTS_BINARY_COUNTER_H

template<typename T, typename I, typename Op>
// requires Op is a BinaryOperation(T)
// and Op is associative
// and I is a ForwardIterator and ValueType(I) == T
T add_to_counter(I first, I last, Op op, const T &zero, T carry) {
    // precondition carry != zero
    while (first != last) {
        if (*first == zero) {
            *first = carry;
            return zero;
        }
        carry = op(*first, carry);
        *first = zero;
        ++first;
    }
    return carry;
}

template<typename T, typename I, typename Op>
// requires Op is a BinaryOperation(T)
// and Op is associative
// and I is a ForwardIterator and ValueType(I) == T
T reduce_counter(I first, I last, Op op, const T &zero) {
    while (first != last && *first == zero) { ++first; }
    if (first == last) return zero;
    T result = *first;
    while (++first != last) {
        if (*first != zero) {
            result = op(*first, result);
        }
    }
    return result;
}

template <typename Op, typename T = typename Op::argument_type>
class binary_counter {
private:
    std::vector<T> counter;
    Op op;
    T zero;

public:
    binary_counter(const Op& op, const T& zero) : op(op), zero(zero) {}
    ~binary_counter() {
        std::cout << "counter size: " << counter.size() << std::endl;
    }

    void reserve(size_t n) { counter.reserve(n); }

    void add(T x) {
        x = add_to_counter(std::begin(counter), std::end(counter), op, zero, x);
        if (x != zero) counter.push_back(x);
    }

    T reduce() {
        return reduce_counter(std::begin(counter), std::end(counter), op, zero);
    }
};

#endif //PROGRAMMINGWITHCOMPONENTS_BINARY_COUNTER_H
