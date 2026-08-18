#ifndef MUTABLE_ARRAY_SEQUENCE_H
#define MUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template <typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {}
    explicit MutableArraySequence(int size) : ArraySequence<T>(size) {}
    MutableArraySequence(T* arr, int count) : ArraySequence<T>(arr, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}
    MutableArraySequence(const Sequence<T>* other) : ArraySequence<T>(other) {}

    MutableArraySequence<T>* CreateEmptySequence() const override {
        return new MutableArraySequence<T>;
    }
    
    MutableArraySequence<T>* Instance() const override {
        return const_cast <MutableArraySequence<T>*>(this);
    }
};

#endif