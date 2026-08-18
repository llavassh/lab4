#ifndef IMMUTABLE_LIST_SEQUENCE_H
#define IMMUTABLE_LIST_SEQUENCE_H

#include "ListSequence.h"

template <typename T>
class ImmutableListSequence: public ListSequence<T> {
private:
    //создание копии последовательности
    ImmutableListSequence<T>* CloneForImmutable() const {
        return new ImmutableListSequence<T>(*this);
    }

public:
    //конструкторы
    ImmutableListSequence(): ListSequence<T>() {}
    ImmutableListSequence(T* arr, int count) : ListSequence<T>(arr, count) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}

    ImmutableListSequence<T>* CreateEmptySequence() const override {
        return new ImmutableListSequence<T>;
    }

    ImmutableListSequence<T>* Instance() const override {
        return CloneForImmutable();
    }
};

#endif