#ifndef PROGRAMMINGWITHCOMPONENTS_INSTRUMENTED_H
#define PROGRAMMINGWITHCOMPONENTS_INSTRUMENTED_H

#include <utility>
#include <ostream>

struct instrumented_base {
    enum operations {
        construction,
        default_construction,
        copy,
        move,
        destruction,
        assignment,
        move_assignment,
        equality,
        comparison,
        put_to
    };


    static double counts[10];
    static const char* counter_names[10];
    static void initialize();
};

template <typename T>
struct instrumented : instrumented_base {
    typedef T value_type;
    T value;

    // conversions
    explicit instrumented(const T& x) : value(x) {
        ++counts[construction];
    }

    // semi-regular
    instrumented() { ++counts[default_construction]; }
    instrumented(const instrumented& x) : value(x.value) { ++counts[copy]; }
    instrumented(instrumented&& x) noexcept : value(std::move(x.value)) { ++counts[move]; }
    ~instrumented() { ++counts[destruction]; }

    instrumented& operator=(const instrumented& x) {
        ++counts[assignment];
        value = x.value;
        return *this;
    }

    instrumented& operator=(instrumented&& x) {
        ++counts[move_assignment];
        value = std::move(x.value);
        return *this;
    }

    // regular
    friend
    bool operator==(const instrumented& x, const instrumented& y) {
        ++counts[equality];
        return x.value == y.value;
    }

    friend
    bool operator!=(const instrumented& x, const instrumented& y) {
        return !(x == y);
    }

    // totally ordered
    friend
    bool operator<(const instrumented& x, const instrumented& y) {
        ++counts[comparison];
        return x.value < y.value;
    }

    friend
    bool operator>(const instrumented& x, const instrumented& y) {
        return y < x;
    }

    friend
    bool operator<=(const instrumented& x, const instrumented& y) {
        return !(y < x);
    }

    friend
    bool operator>=(const instrumented& x, const instrumented& y) {
        return !(x < y);
    }

    friend
    std::ostream& operator<<(std::ostream& os, const instrumented& x) {
        ++counts[put_to];
        return os << "instrumented(" << x.value << ")";
    }
};

#endif //PROGRAMMINGWITHCOMPONENTS_INSTRUMENTED_H
