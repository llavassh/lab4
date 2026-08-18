#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Exceptions.h"
#include "IEnumerator.h"
#include "Option.h"

template <typename T>
class Sequence {
public:
    virtual ~Sequence() = default;

    //декомпозиция
    virtual const T& Get(int index) const = 0;
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual int GetLength() const = 0;

    //try-семантика
    Option<T> TryGet(int index) const {
        try { 
            return Option<T>::Some(Get(index)); 
        } catch(...) { 
            return Option<T>::None(); 
        }
    }

    Option<T> TryGetFirst() const {
        try { 
            return Option<T>::Some(GetFirst()); 
        } catch(...) { 
            return Option<T>::None(); 
        }
    }

    Option<T> TryGetLast() const {
        try { 
            return Option<T>::Some(GetLast()); 
        } catch(...) { 
            return Option<T>::None(); 
        }
    }

    virtual Sequence<T>* Append(const T &item) const = 0;
    virtual Sequence<T>* Prepend(const T &item) const = 0;
    virtual Sequence<T>* InsertAt(int index, const T &item) const = 0;
    virtual Sequence<T>* RemoveAt(int index) const = 0;
    virtual Sequence<T>* GetSubSequence(int start, int end) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;
    virtual Sequence<T>* Slice(int start, int count, const Sequence<T>* insertSeq = nullptr) const = 0;

    virtual IEnumerator<T>* GetEnumerator() const = 0;

    virtual Sequence<T>* Instance() const = 0;

    const T& operator[](int index) const {
        return Get(index);
    }

    //преобразует элементы
    Sequence<T>* Map(T (*func)(const T&)) const {
        Sequence<T>* result = CreateEmptySequence();
        for (int i = 0; i < GetLength(); ++i) {
            Sequence<T>* next = result->Append(func(Get(i)));
            if (next != result) { 
                delete result; 
                result = next; 
            }
        }
        return result; 
    }

    //map с индексом
    Sequence<T>* MapWithIndex(T (*func)(const T&, int)) const {
        Sequence<T>* result = CreateEmptySequence();
        for (int i = 0; i < GetLength(); ++i) {
            Sequence<T>* next = result->Append(func(Get(i), i));
            if (next != result) { 
                delete result;
                result = next; 
            }
        }
        return result;
    }

    //фильтрует элементы
    Sequence<T>* Where(bool (*predicate)(const T&)) const {
        Sequence<T>* result = CreateEmptySequence();
        for (int i = 0; i < GetLength(); ++i) {
            const T& val = Get(i);
            if (predicate(val)) {
                Sequence<T>* next = result->Append(val);
                if (next != result) { 
                    delete result; 
                    result = next; 
                }
            }
        }
        return result;
    }

    //каждый элемент порождает последовательность, все склеиваются
    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const {
        Sequence<T>* result = CreateEmptySequence();
        for (int i = 0; i < GetLength(); ++i) {
            Sequence<T>* sub = func(Get(i));
            for (int j = 0; j < sub->GetLength(); ++j) {
                Sequence<T>* next = result->Append(sub->Get(j));
                if (next != result) { 
                    delete result; 
                    result = next; 
                }
            }
            delete sub;
        }
        return result;
    }

    //ищет первый подходящий элемент
    Option<T> Find(bool (*predicate)(const T&)) const {
        for (int i = 0; i < GetLength(); ++i) {
            const T& val = Get(i);
            if (predicate(val)) {
                return Option<T>::Some(val);
            }
        }
        return Option<T>::None();
    }

    Option<T> TryFind(bool (*predicate)(const T&)) const {
        return Find(predicate);
    }

    //сводит все элементы к одному
    T Reduce(T (*func)(const T&, const T&), T initial) const {
        T result = initial;
        for (int i = 0; i < GetLength(); i++)
            result = func(result, Get(i));
        return result;
    }

    //reduce с индексом
    T ReduceWithIndex(T (*func)(const T&, const T&, int), T initial) const {
        T result = initial;
        for (int i = 0; i < GetLength(); i++)
            result = func(result, Get(i), i);
        return result;
    }

    //пропустить первые n элементов
    Sequence<T>* Skip(int count) const {
        if (count < 0) count = 0;
        if (count >= GetLength()) return CreateEmptySequence();
        return GetSubSequence(count, GetLength() - 1);
    }

protected:
    virtual Sequence<T>* CreateEmptySequence() const = 0;

};

#endif