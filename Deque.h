#ifndef DEQUE_H
#define DEQUE_H

#include "Sequence.h"
#include "MutableListSequence.h"
#include "Exceptions.h"
#include <ostream>

template <typename T>
class Deque {
private:
    Sequence<T> *seq;

public:
    Deque() : seq(new MutableListSequence<T>()) {}

    Deque(const T* arr, int count) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < count; i++) {
            PushBack(arr[i]);
        }
    }

    Deque(const Deque<T>& other) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < other.GetSize(); i++) {
            PushBack(other.Get(i));
        }
    }

    ~Deque() {
        delete seq;
    }

    Deque<T>& operator=(const Deque<T>& other) {
        if (this != &other) {
            delete seq;
            seq = new MutableListSequence<T>();
            for (int i = 0; i < other.GetSize(); i++) {
                PushBack(other.Get(i));
            }
        }
        return *this;
    }

    void PushFront(const T& item) {
        seq->Prepend(item);
    }

    void PushBack(const T& item) {
        seq->Append(item);
    }

    T PopFront() {
        if (IsEmpty()) throw EmptyContainer("Deque");
        T item = seq->GetFirst();
        seq->RemoveAt(0);
        return item;
    }

    T PopBack() {
        if (IsEmpty()) throw EmptyContainer("Deque");
        T item = seq->GetLast();
        seq->RemoveAt(seq->GetLength() - 1);
        return item;
    }

    const T& PeekFront() const {
        if (IsEmpty()) throw EmptyContainer("Deque");
        return seq->GetFirst();
    }

    const T& PeekBack() const {
        if (IsEmpty()) throw EmptyContainer("Deque");
        return seq->GetLast();
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

    int GetSize() const {
        return seq->GetLength();
    }

    const T& Get(int index) const {
        if (index < 0 || index >= GetSize()) throw IndexOutOfRange(index, GetSize());
        return seq->Get(index);
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); i++) {
            result.PushBack(Get(i));
        }
        for (int i = 0; i < other.GetSize(); i++) {
            result.PushBack(other.Get(i));
        }
        return result;
    }

    Deque<T> GetSubSequence(int start, int end) const {
        if (start < 0 || end >= GetSize() || start > end) {
            throw IndexOutOfRange(start, GetSize());
        }
        Deque<T> result;
        for (int i = start; i <= end; i++) {
            result.PushBack(Get(i));
        }
        return result;
    }

    int FindSubSequence(const Deque<T> &pattern) const {
        int n = GetSize();
        int m = pattern.GetSize();
        if (m == 0)
            return 0;
        if (m > n)
            return -1;

        int startIdx = 0;
        while (startIdx <= n - m) {
            bool match = true;
            for (int i = 0; i < m; i++) {
                if (Get(startIdx + i) != pattern.Get(i)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return startIdx;
            }
            startIdx++;
        }
        return -1;
    }

    void Split(bool (*predicate)(const T&), Deque<T>& Deque1, Deque<T>& Deque2) const {
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                Deque1.PushBack(Get(i));
            } else {
                Deque2.PushBack(Get(i));
            }
        }
    }

    Deque<T> Sort(bool (*less)(const T&, const T&)) const {
        if (GetSize() <= 1) return *this;
        
        T* arr = new T[GetSize()];
        for (int i = 0; i < GetSize(); i++) {
            arr[i] = Get(i);
        }
        
        for (int i = 0; i < GetSize() - 1; i++) {
            for (int j = 0; j < GetSize() - i - 1; j++) {
                if (!less(arr[j], arr[j + 1])) {
                    T temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
        
        Deque<T> result;
        for (int i = 0; i < GetSize(); i++) {
            result.PushBack(arr[i]);
        }
        delete[] arr;
        return result;
    }

    Deque<T> Map(T (*func)(const T&)) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); i++){
            result.PushBack(func(Get(i)));
        }
        return result;
    }

    Deque<T> Where(bool (*predicate)(const T&)) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                result.PushBack(Get(i));
            }
        }
        return result;
    }

    T Reduce(T (*func)(const T&, const T&), T initial) const {
        T result = initial;
        for (int i = 0; i < GetSize(); i++) {
            result = func(result, Get(i));
        }
        return result;
    }

    Deque<T> Merge(const Deque<T>& other, bool (*less)(const T&, const T&)) const {
        Deque<T> result;
        int i = 0;
        int j = 0;
        while (i < GetSize() && j < other.GetSize()) {
            if (less(Get(i), other.Get(j))) {
                result.PushBack(Get(i));
                i++;
            } else {
                result.PushBack(other.Get(j));
                j++;
            }
        }

        while (i < GetSize()) {
            result.PushBack(Get(i));
            i++;
        }

        while (j < other.GetSize()) {
            result.PushBack(other.Get(j));
            j++;
        }
        
        return result;
    }

    Deque<T> operator+(const Deque<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Deque<T>& other) const {
        if (GetSize() != other.GetSize()) return false;
        for (int i = 0; i < GetSize(); i++) {
            if (Get(i) != other.Get(i))
                return false;
        }
        return true;
    }

    bool operator!=(const Deque<T>& other) const {
        return !(*this == other);
    }

    const T& operator[](int index) const {
        return Get(index);
    }

    friend std::ostream& operator<<(std::ostream& out, const Deque<T>& deque) {
        out << "[";
        for (int i = 0; i < deque.GetSize(); i++) {
            out << deque.Get(i);
            if (i < deque.GetSize() - 1)
                out << ", ";
        }
        out << "]";
        return out;
    }  
};

#endif