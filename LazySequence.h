#ifndef LAZYSEQUENCE_H
#define LAZYSEQUENCE_H

#include "MutableArraySequence.h"
#include "Generator.h"
#include "Ordinal.h"
#include "Exceptions.h"

template <typename T>
class LazySequence {
    friend class RuleGenerator<T>;

private:
    MutableArraySequence<T> cache;
    Generator<T> *generator;
    Ordinal length;

public:
    LazySequence() : cache(), generator(nullptr), length(Ordinal(0, 0)) {}
    LazySequence(T* items, int count) : cache(), generator(new SequenceGenerator<T>(items, count)), 
        length(Ordinal(0, count)) {}
    LazySequence(Sequence<T> *seq) : cache(), generator(new SequenceGenerator<T>(seq)), 
        length(Ordinal(0, seq->GetLength())) {}
    LazySequence(T (*rule)(Sequence<T>*), Sequence<T>* initial): cache(initial), 
        generator(new RuleGenerator<T>(this, rule, initial)), length(Ordinal(1, 0)) {}
    LazySequence(const LazySequence& other) : cache(other.cache), generator(other.generator == nullptr ? nullptr : other.generator->Clone(this)),
        length(other.length) {}

    ~LazySequence() {
        delete generator;
    }

    LazySequence<T>& operator=(const LazySequence& other) {
        if (this != &other) {
            Generator<T>* newGenerator = (other.generator == nullptr) ? nullptr : other.generator->Clone(this);
            delete generator;
            generator = newGenerator;
            cache = other.cache;
            length = other.length;
        }
        return *this;
    }
    
    T Get(Ordinal index) {
        if (GetLength() <= index)
            throw OrdinalIndexOutOfRange(index);
        if (index.GetOmegaCount() == 0) {
            int count = index.GetFiniteSize();
            if (cache.GetLength() > count)
                return cache.Get(count);
            while (cache.GetLength() <= count)
            {
                T next = generator->GetNext();
                cache.Append(next);
            }
            return cache.Get(count);
        }
        return generator->Get(index);
    }

    T GetFirst() {
        if (length == Ordinal(0, 0))
            throw EmptyContainer("LazySequence");
        return Get(Ordinal(0, 0));
    }

    T GetLast() {
        if (length == Ordinal(0, 0))
            throw EmptyContainer("LazySequence");
        if (length.GetOmegaCount() > 0 && length.GetFiniteSize() == 0)
            throw std::logic_error("Infinite LazySequence has no last element");
        return Get(Ordinal(length.GetOmegaCount(), length.GetFiniteSize() - 1));
    }

    Ordinal GetLength() const {
        return length;
    }

    int GetMaterializedCount() const {
        return cache.GetLength();
    }

    LazySequence<T> *InsertAt(Ordinal index, T item) const;
    LazySequence<T> *Append(const T &item) const;
    LazySequence<T> *Prepend(const T &item) const;
    LazySequence<T> *RemoveAt(Ordinal index) const;
    LazySequence<T> *Concat(const LazySequence<T>& other) const;
    LazySequence<T> *Map(T (*func)(const T &)) const;
    LazySequence<T> *GetSubSequence(Ordinal start, Ordinal end) const;
};

template <typename T> 
class InsertGenerator : public Generator<T> {
private:
    LazySequence<T>* owner;
    LazySequence<T> source;
    Ordinal modificationIndex;
    int currentIndex;
    T item;

public:
    InsertGenerator(LazySequence<T> *owner, LazySequence<T> source, Ordinal modificationIndex, T item) : owner(owner),
    source(source), modificationIndex(modificationIndex), currentIndex(0), item(item) {}

    T GetNext() override {
        if (!HasNext())
                throw OrdinalIndexOutOfRange(modificationIndex);
        if (modificationIndex.GetOmegaCount() == 0) {
            int intIndex = modificationIndex.GetFiniteSize();
            if (currentIndex < intIndex) {
                T next = source.Get(Ordinal(0, currentIndex));
                currentIndex++;
                return next;
            }
            if (currentIndex == intIndex) {
                currentIndex++;
                return item;
            }
            else {
                T next = source.Get(Ordinal(0, currentIndex - 1));
                currentIndex++;
                return next;
            }
        }
        T next = source.Get(Ordinal(0, currentIndex));
        currentIndex++;
        return next;
    }

    T Get(Ordinal index) override {
        if (index < modificationIndex)
            return source.Get(index);
        if (index == modificationIndex)
            return item;
        else {
            if (index.GetOmegaCount() == modificationIndex.GetOmegaCount())
                return source.Get(Ordinal(index.GetOmegaCount(), index.GetFiniteSize() - 1));
            return source.Get(index);
        }
    }

    bool HasNext() const override {
        if (owner->GetLength().GetOmegaCount() == 0) {
            return currentIndex < owner->GetLength().GetFiniteSize();
        }
        return true;
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch(...) {
            return Option<T>::None();
        }
    }

    InsertGenerator<T>* Clone(LazySequence<T>* newOwner) const override {
        InsertGenerator<T> *copy = new InsertGenerator(newOwner, source, modificationIndex, item);
        copy->currentIndex = currentIndex;
        return copy;
    }
};


template <typename T>
class RemoveGenerator: public Generator<T> {
private:
    LazySequence<T>* owner;
    LazySequence<T> source;
    Ordinal modificationIndex;
    int currentIndex;

public:
    RemoveGenerator(LazySequence<T> *owner, LazySequence<T> source, Ordinal modificationIndex) : owner(owner), source(source),
    modificationIndex(modificationIndex), currentIndex(0) {}

    T GetNext() override {
        if (!HasNext())
            throw OrdinalIndexOutOfRange(modificationIndex);
        if (modificationIndex.GetOmegaCount() == 0) {
            int intIndex = modificationIndex.GetFiniteSize();
            if (currentIndex < intIndex) {
                T next = source.Get(Ordinal(0, currentIndex));
                currentIndex++;
                return next;
            }
            if (currentIndex >= intIndex) {
                T next = source.Get(Ordinal(0, currentIndex + 1));
                currentIndex++;
                return next;
            }
        }
        T next = source.Get(Ordinal(0, currentIndex));
        currentIndex++;
        return next;
    }

    T Get(Ordinal index) override {
        if (index < modificationIndex)
            return source.Get(index);
        else {
            if (index.GetOmegaCount() == modificationIndex.GetOmegaCount())
                return source.Get(Ordinal(index.GetOmegaCount(), index.GetFiniteSize() + 1));
            return source.Get(index);
        }
    }

    bool HasNext() const override {
        if (owner->GetLength().GetOmegaCount() == 0) {
            return currentIndex < owner->GetLength().GetFiniteSize();
        }
        return true;
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch (...) {
            return Option<T>::None();
        }
    }

    Generator<T>* Clone(LazySequence<T>* newOwner) const override {
        RemoveGenerator<T> *copy = new RemoveGenerator(newOwner, source, modificationIndex);
        copy->currentIndex = currentIndex;
        return copy;
    }
};

template <typename T>
class ConcatGenerator : public Generator<T> {
private:
    LazySequence<T> *owner;
    LazySequence<T> first;
    LazySequence<T> second;
    int currentIndex;

public:
    ConcatGenerator(LazySequence<T>* owner, LazySequence<T> first, LazySequence<T> second) : owner(owner), first(first), 
    second(second), currentIndex(0) {}

    T GetNext() override {
        if (!HasNext())
            throw IndexOutOfRange(currentIndex, owner->GetLength().GetFiniteSize());
        if (first.GetLength().GetOmegaCount() == 0) {
            if (currentIndex < first.GetLength().GetFiniteSize()) {
                T next = first.Get(Ordinal(0, currentIndex));
                currentIndex++;
                return next;
            }
            else {
                T next = second.Get(Ordinal(0, currentIndex - first.GetLength().GetFiniteSize()));
                currentIndex++;
                return next;
            }
        }
        T next = first.Get(Ordinal(0, currentIndex));
        currentIndex++;
        return next;
    }

    T Get(Ordinal index) override {
        if (owner->GetLength() <= index)
            throw OrdinalIndexOutOfRange(index);
        if (index < first.GetLength()) {
            return first.Get(index);
        }
        if (index.GetOmegaCount() == first.GetLength().GetOmegaCount()) {
            return second.Get(Ordinal(0, index.GetFiniteSize() - first.GetLength().GetFiniteSize()));
        }
        return second.Get(Ordinal(index.GetOmegaCount() - first.GetLength().GetOmegaCount(), index.GetFiniteSize()));
    }

    bool HasNext() const override {
        if (first.GetLength().GetOmegaCount() != 0 || second.GetLength().GetOmegaCount() != 0)
            return true;
        else {
            return currentIndex < first.GetLength().GetFiniteSize() + second.GetLength().GetFiniteSize();
        }
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch (...) {
            return Option<T>::None();
        }
    }

    Generator<T>* Clone(LazySequence<T>* newOwner) const override {
        ConcatGenerator<T> *copy = new ConcatGenerator<T>(newOwner, first, second);
        copy->currentIndex = currentIndex;
        return copy;
    }
};

template <typename T>
class SubSequenceGenerator : public Generator<T> {
private:
    LazySequence<T> *owner;
    LazySequence<T> source;
    Ordinal start;
    Ordinal end;
    int currentIndex;

public:
    SubSequenceGenerator(LazySequence<T>* owner, LazySequence<T> source, Ordinal start, Ordinal end) : owner(owner), source(source), 
    start(start), end(end), currentIndex(0) {}
    
    T GetNext() override {
        if (!HasNext())
            throw IndexOutOfRange(currentIndex, source.GetLength().GetFiniteSize());
        T next = source.Get(Ordinal(start.GetOmegaCount(), start.GetFiniteSize() + currentIndex));
        currentIndex++;
        return next;
    }

    T Get(Ordinal index) override {
        if (owner->GetLength() <= index)
            throw OrdinalIndexOutOfRange(index);
        return source.Get(start + index);
    }

    bool HasNext() const override {
        if (owner->GetLength().GetOmegaCount() != 0) {
            return true;
        }
        return currentIndex < owner->GetLength().GetFiniteSize();
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch(...) {
            return Option<T>::None();
        }
    }

    Generator<T>* Clone(LazySequence<T>* newOwner) const override {
        SubSequenceGenerator<T> *copy = new SubSequenceGenerator<T>(newOwner, source, start, end);
        copy->currentIndex = currentIndex;
        return copy;
    }
};

template <typename T>
class MapGenerator : public Generator<T> {
private:
    LazySequence<T> *owner;
    LazySequence<T> source;
    T (*func)(const T &);
    int currentIndex;

public:
    MapGenerator(LazySequence<T>* owner, LazySequence<T> source, T (*func)(const T&)) : owner(owner), source(source), func(func), 
    currentIndex(0) {}

    T GetNext() override {
        if (!HasNext())
            throw IndexOutOfRange(currentIndex, source.GetLength().GetFiniteSize());
        T next = func(source.Get(Ordinal(0, currentIndex)));
        currentIndex++;
        return next;
    }

    T Get(Ordinal index) override {
        if (owner->GetLength() <= index)
            throw OrdinalIndexOutOfRange(index);
        return func(source.Get(index));
    }

    bool HasNext() const override {
        if (source.GetLength().GetOmegaCount() != 0) {
            return true;
        }
        return currentIndex < source.GetLength().GetFiniteSize();
    }

    Option<T> TryGetNext() override {
        try {
            return Option<T>::Some(GetNext());
        } catch(...) {
            return Option<T>::None();
        }
    }

    Generator<T>* Clone(LazySequence<T>* newOwner) const override {
        MapGenerator<T> *copy = new MapGenerator<T>(newOwner, source, func);
        copy->currentIndex = currentIndex;
        return copy;
    }
};

template <typename T>
LazySequence<T>* LazySequence<T>::InsertAt(Ordinal index, T item) const {
    if (GetLength() < index)
        throw OrdinalIndexOutOfRange(index);
    LazySequence<T> *result = new LazySequence<T>();
    result->length = length + Ordinal(0, 1);
    result->generator = new InsertGenerator<T>(result, *this, index, item);
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Append(const T& item) const {
    return InsertAt(GetLength(), item);
}


template <typename T>
LazySequence<T>* LazySequence<T>::Prepend(const T& item) const {
    return InsertAt(Ordinal(0, 0), item);
}

template <typename T>
LazySequence<T>* LazySequence<T>::RemoveAt(Ordinal index) const {
    if (length <= index)
        throw OrdinalIndexOutOfRange(index);
    LazySequence<T> *result = new LazySequence<T>(); 
    if (length.GetFiniteSize() == 0) {
        result->length = Ordinal(length.GetOmegaCount(), length.GetFiniteSize());
    }
    else {
        result->length = Ordinal(length.GetOmegaCount(), length.GetFiniteSize() - 1);
    }
    result->generator = new RemoveGenerator<T>(result, *this, index);
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Concat(const LazySequence<T>& other) const {
    LazySequence<T> *result = new LazySequence<T>();
    result->generator = new ConcatGenerator<T>(result, *this, other);
    result->length = GetLength() + other.GetLength();
    return result;
}

template <typename T> 
LazySequence<T>* LazySequence<T>::GetSubSequence(Ordinal start, Ordinal end) const {
    if (end < start || GetLength() <= end)
        throw OrdinalIndexOutOfRange(end);
    LazySequence<T> *result = new LazySequence<T>();
    result->generator = new SubSequenceGenerator<T>(result, *this, start, end);
    if (end.GetOmegaCount() == start.GetOmegaCount())
        result->length = Ordinal(0, end.GetFiniteSize() - start.GetFiniteSize() + 1);
    else {
        result->length = Ordinal(end.GetOmegaCount() - start.GetOmegaCount(), end.GetFiniteSize() + 1);
    }
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Map(T (*func)(const T&)) const {
    LazySequence<T> *result = new LazySequence<T>();
    result->generator = new MapGenerator<T>(result, *this, func);
    result->length = length;
    return result;
}

#endif