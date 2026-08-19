#ifndef GENERATOR_H
#define GENERATOR_H

#include "Option.h"
#include "MutableArraySequence.h"
#include "Ordinal.h"

template <typename T>
class LazySequence;

template <typename T>
class Generator {
public:
    virtual bool HasNext() const = 0;
    virtual T GetNext() = 0;
    virtual T Get(Ordinal index) = 0;
    virtual Option<T> TryGetNext() = 0;
    virtual Generator<T> *Clone(LazySequence<T>* newOwner) const = 0;
    virtual ~Generator() = default;
};

template <typename T>
class SequenceGenerator: public Generator<T> {
private:
    int currentIndex;
    Sequence<T> *seq;

public:
    SequenceGenerator(T* items, int count) : currentIndex(0), seq(new MutableArraySequence<T>(items, count)) {}
    SequenceGenerator(Sequence<T>* seq) : currentIndex(0), seq(new MutableArraySequence<T>(seq)) {}
    ~SequenceGenerator() {
        delete seq;
    }

    T GetNext() override {
        if (!HasNext())
            throw IndexOutOfRange(currentIndex, seq->GetLength());
        T next = seq->Get(currentIndex);
        currentIndex++;
        return next;
    }
    
    T Get(Ordinal index) override {
        if (index.GetOmegaCount() != 0)
            throw OrdinalIndexOutOfRange(index);
        return seq->Get(index.GetFiniteSize());
    }

    bool HasNext() const override {
        return currentIndex < seq->GetLength();
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch (...) {
            return Option<T>::None();
        }
    }

    SequenceGenerator<T>* Clone(LazySequence<T>*) const override {
        SequenceGenerator<T> *copy = new SequenceGenerator<T>(seq);
        copy->currentIndex = currentIndex;
        return copy;
    }
};

template <typename T>
class RuleGenerator: public Generator<T> {
private:
    LazySequence<T>* owner;
    T (*rule)(Sequence<T>*);
    MutableArraySequence<T> state;

public:

    RuleGenerator(LazySequence<T>* owner, T (*rule)(Sequence<T>*), const Sequence<T>* initial) : owner(owner), rule(rule), 
       state(initial) {}

    T GetNext() override {
        if (state.GetLength() == 0)
            throw EmptyContainer("LazySequence");
        T next = rule(&state);
        state.RemoveAt(0);
        state.Append(next);
        return next;
    }

    T Get(Ordinal index) override {
        if (index.GetOmegaCount() != 0)
            throw OrdinalIndexOutOfRange(index);
        return owner->Get(index);
    }

    bool HasNext() const override {
        return true;
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        }
        catch (...) {
            return Option<T>::None();
        }
    }

    RuleGenerator<T>* Clone(LazySequence<T>* newOwner) const override {
        RuleGenerator<T> *copy = new RuleGenerator(newOwner, rule, &state);
        return copy;
    } 
};

#endif