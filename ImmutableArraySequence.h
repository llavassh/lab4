#ifndef IMMUTABLE_ARRAY_SEQUENCE_H
#define IMMUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
private:
    //создание копии последовательности
    ImmutableArraySequence<T>* CloneForImmutable() const {
        return new ImmutableArraySequence(*this);
    }

public:
    //конструкторы
    ImmutableArraySequence() : ArraySequence<T>() {}
    explicit ImmutableArraySequence(int size) : ArraySequence<T>(size) {}
    ImmutableArraySequence(T* arr, int count) : ArraySequence<T>(arr, count) {}
    ImmutableArraySequence(const DynamicArray<T>& arr) : ArraySequence<T>(arr) {}

    ImmutableArraySequence<T>* CreateEmptySequence() const override {
        return new ImmutableArraySequence<T>;
    }

    ImmutableArraySequence<T>* Instance() const override {
        return CloneForImmutable();
    }
    
};

#endif