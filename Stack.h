#ifndef STACK_H
#define STACK_H

#include "Sequence.h"
#include "MutableListSequence.h"
#include "Exceptions.h"
#include <ostream>

template <typename T>
class Stack {
private:
    Sequence<T> *seq;

public:
    Stack() : seq(new MutableListSequence<T>()) {} 
       
    Stack(const T* arr, int count) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < count; i++) {
            Push(arr[i]);
        }
    }

    Stack(const Stack<T>& other) : seq(new MutableListSequence<T>()) {
        for (int i = 0; i < other.GetSize(); i++) {
            Push(other.Get(i));
        }
    }

    ~Stack() {
        delete seq;
    }

    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {
            delete seq;
            seq = new MutableListSequence<T>();
            for (int i = 0; i < other.GetSize(); i++) {
                Push(other.Get(i));
            }
        }
        return *this;
    }

    void Push(const T& item) {
        seq->Append(item);
    }

    T Pop() {
        if (IsEmpty()) throw EmptyContainer("Stack");
        T item = seq->GetLast();
        seq->RemoveAt(seq->GetLength() - 1);
        return item;
    }

    const T& Peek() const {
        if (IsEmpty()) throw EmptyContainer("Stack");
        return seq->GetLast();
    }

    int GetSize() const {
        return seq->GetLength();
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

    const T& Get(int index) const {
        if (index < 0 || index >= GetSize()) throw IndexOutOfRange(index, GetSize());
        return seq->Get(index);
    }

    Stack<T> Concat(const Stack<T>& other) const {
        Stack<T> result;
        for (int i = 0; i < GetSize(); i++) {
            result.Push(Get(i));
        }
        for (int i = 0; i < other.GetSize(); i++) {
            result.Push(other.Get(i));
        }
        return result;
    }

    Stack<T> GetSubSequence(int start, int end) const {
        if (start < 0 || end >= GetSize() || start > end) {
            throw IndexOutOfRange(start, GetSize());
        }
        Stack<T> result;
        for (int i = start; i <= end; i++) {
            result.Push(Get(i));
        }
        return result;
    }

    int FindSubSequence(const Stack<T>& pattern) const {
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

    void Split(bool (*predicate)(const T&), Stack<T>& Stack1, Stack<T>& Stack2) const {
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                Stack1.Push(Get(i));
            }
            else {
                Stack2.Push(Get(i));
            }
        }
    }

    Stack<T> Map(T (*func)(const T&)) const {
        Stack<T> result;
        for (int i = 0; i < GetSize(); i++) {
            result.Push(func(Get(i)));
        }
        return result;
    }

    Stack<T> Where(bool (*predicate)(const T&)) const {
        Stack<T> result;
        for (int i = 0; i < GetSize(); i++) {
            if (predicate(Get(i))) {
                result.Push(Get(i));
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

    Stack<T> operator+(const Stack<T>& other) const {
        return Concat(other);
    }

    bool operator==(const Stack<T>& other) const {
        if (GetSize() != other.GetSize()) return false;
        for (int i = 0; i < GetSize(); i++) {
            if (Get(i) != other.Get(i))
                return false;
        }
        return true;
    }

    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }

    const T& operator[](int index) const {
        return Get(index);
    }

    friend std::ostream& operator<<(std::ostream& out, const Stack<T>& stack) {
        out << "[";
        for (int i = 0; i < stack.GetSize(); i++) {
            out << stack.Get(i);
            if (i < stack.GetSize() - 1)
                out << ", ";
        }
        out << "]";
        return out;
    }   
};

#endif