#ifndef QUEUE_H
#define QUEUE_H

#include "Sequence.h"
#include "MutableListSequence.h"
#include "Exceptions.h"
#include "Zip.h"
#include <ostream>

template <typename T>
class Queue {
private:
    Sequence<T> *seq;

public:
    Queue() : seq(new MutableListSequence<T>()) {}

    Queue(const T* arr, int count) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < count; i++) {
            Enqueue(arr[i]);
        }
    }

    Queue(const Queue<T>& other) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < other.GetSize(); i++) {
            Enqueue(other.Get(i));
        }
    }

    ~Queue() {
        delete seq;
    }

    Queue<T>& operator=(const Queue<T>& other) {
        if (this != &other) {
            delete seq;
            seq = new MutableListSequence<T>;
            for (int i = 0; i < other.GetSize(); i++) {
                Enqueue(other.Get(i));
            }
        }
        return *this;
    }

    void Enqueue(const T& item) {
        seq->Append(item);
    }

    T Dequeue() {
        if (IsEmpty()) throw EmptyContainer("Queue");
        T item = seq->GetFirst();
        seq->RemoveAt(0);
        return item;
    }

    const T& Peek() const {
        if (IsEmpty()) throw EmptyContainer("Queue");
        return seq->GetFirst();
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

    Queue<T> Concat(const Queue<T>& other) const {
        Queue<T> result;
        for (int i = 0; i < GetSize(); i++) {
            result.Enqueue(Get(i));
        }
        for (int i = 0; i < other.GetSize(); i++) {
            result.Enqueue(other.Get(i));
        }
        return result;
    }

    Queue<T> GetSubSequence(int start, int end) const {
        if (start < 0 || end >= GetSize() || start > end) {
            throw IndexOutOfRange(start, GetSize());
        }
        Queue<T> result;
        for (int i = start; i <= end; i++) {
            result.Enqueue(Get(i));
        }
        return result;
    }

    int FindSubSequence(const Queue<T>& pattern) const {
        int n = GetSize();
        int m = pattern.GetSize();
        if (m == 0) return 0;
        if (m > n) return -1;

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

    void Split(bool (*predicate)(const T&), Queue<T>& Queue1, Queue<T>& Queue2) const {
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                Queue1.Enqueue(Get(i));
            } else {
                Queue2.Enqueue(Get(i));
            }
        }
    }

    Queue<ZipPair<T, T>> Zip(const Queue<T>& other) const {
        Queue<ZipPair<T, T>> result;
        int minSize = (GetSize() < other.GetSize()) ? GetSize() : other.GetSize();
        for (int i = 0; i < minSize; i++) {
            result.Enqueue(ZipPair<T, T>(Get(i), other.Get(i)));
        }
        return result;
    }

    Queue<T> Map(T (*func)(const T&)) const {
        Queue<T> result;
        for (int i = 0; i < GetSize(); i++){
            result.Enqueue(func(Get(i)));
        }
        return result;
    }

    Queue<T> Where(bool (*predicate)(const T&)) const {
        Queue<T> result;
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                result.Enqueue(Get(i));
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

    Queue<T> operator+(const Queue<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Queue<T>& other) const {
        if (GetSize() != other.GetSize()) return false;
        for (int i = 0; i < GetSize(); i++) {
            if (Get(i) != other.Get(i))
                return false;
        }
        return true;
    }

    bool operator!=(const Queue<T>& other) const {
        return !(*this == other);
    }

    const T& operator[](int index) const {
        return Get(index);
    }

    friend std::ostream& operator<<(std::ostream& out, const Queue<T>& queue) {
        out << "[";
        for (int i = 0; i < queue.GetSize(); i++) {
            out << queue.Get(i);
            if (i < queue.GetSize() - 1)
                out << ", ";
        }
        out << "]";
        return out;
    }  
};

#endif