#ifndef PROGRAMMINGWITHCOMPONENTS_UTIL_H
#define PROGRAMMINGWITHCOMPONENTS_UTIL_H

#include <ostream>

template <typename I>
I successor(I i) {
    I j = i;
    return ++j;
}

template <typename I, typename Join = std::string_view>
std::ostream& join(std::ostream& os, I first, I last, Join j = ", ") {
    while (first != last) {
        os << *first;
        if (successor(first++) != last) os << j;
    }
    return os;
}

#endif //PROGRAMMINGWITHCOMPONENTS_UTIL_H
