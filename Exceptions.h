#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
 
#include <string>
#include <stdexcept>
#include "Ordinal.h"

//показывает ошибку, когда индекс не подходит под размер контейнера
class IndexOutOfRange: public std::out_of_range {
public:
    IndexOutOfRange(int index, int size) : std::out_of_range("Index " + std::to_string(index) + 
    " out of range (size: " + std::to_string(size) + ")") {}
};

class OrdinalIndexOutOfRange: public std::out_of_range {
public:
    OrdinalIndexOutOfRange(Ordinal index) : std::out_of_range("Index (w * " + std::to_string(index.GetOmegaCount()) +
    " + " + std::to_string(index.GetFiniteSize()) + ") out of range ") {}
};

//показывает ошибку, когда контейнер пуст
class EmptyContainer: public std::logic_error {
public:
    explicit EmptyContainer(const std::string &name)
    :std::logic_error(name + " is empty") {}
};

#endif