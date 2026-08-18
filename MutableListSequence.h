#ifndef MUTABLELISTSEQUENCE_H
#define MUTABLELISTSEQUENCE_H

#include "ListSequence.h"

template <typename T>
class MutableListSequence: public ListSequence<T> {
public: 
    MutableListSequence(): ListSequence<T>() {}
    MutableListSequence(T* arr, int count): ListSequence<T>(arr, count) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}

    MutableListSequence<T>* CreateEmptySequence() const override {
        return new MutableListSequence<T>;
    }

    MutableListSequence<T>* Instance() const override { 
        return const_cast <MutableListSequence<T>*>(this); 
    }

};

#endif 