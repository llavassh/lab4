#ifndef ZIP_H
#define ZIP_H

#include <iostream>

template <typename A, typename B>
struct ZipPair {
    A first;
    B second;
    ZipPair(): first(), second() {}
    ZipPair(const A& f, const B& s) : first(f), second(s) {}
};

template <typename A, typename B>
std::ostream& operator<<(std::ostream& os, const ZipPair<A, B>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

#endif