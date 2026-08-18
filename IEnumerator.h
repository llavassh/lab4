#ifndef IENUMERATOR_H
#define IENUMERATOR_H

#include "Option.h"

template <typename T>
class IEnumerator {
public:
    virtual ~IEnumerator() = default;
    virtual bool MoveNext() = 0; //переход к следующему элементу
    virtual T Current() const = 0;  //получение текущего элемента
    virtual void Reset() = 0;  //сбросить в начало

    virtual Option<T> TryCurrent() const {
        try {
            return Option<T>::Some(Current());
        } catch (...) {
            return Option<T>::None();
        }
    }
};

//итератор для dynamicarray
#include "DynamicArray.h"
template <typename T>
class ArrayEnumerator : public IEnumerator<T> {
private:
    const DynamicArray<T>* array;
    int position;
    
public:
    ArrayEnumerator(const DynamicArray<T>* arr) : array(arr), position(-1) {}
    
    bool MoveNext() override {
        position++;
        return position < array->GetSize();
    }
    
    T Current() const override {
        return array->Get(position);
    }
    
    void Reset() override {
        position = -1;
    }
};

//итератор для linkedlist
template <typename T> class LinkedList;

template <typename T>
class ListEnumerator : public IEnumerator<T> {
private:
    const LinkedList<T>* list;
    typename LinkedList<T>::Node* current;
    int position;
    
public:
    ListEnumerator(const LinkedList<T>* lst);
    bool MoveNext() override;
    T Current() const override;
    void Reset() override;
};

#include "LinkedList.h"

template <typename T>
ListEnumerator<T>::ListEnumerator(const LinkedList<T>* lst) : list(lst), current(nullptr), position(-1) {}

template <typename T>
bool ListEnumerator<T>::MoveNext() {
    if (position == -1) {
        current = list->head;
        position = 0;
    } else if (current) {
        current = current->next;
        position++;
    }
    return current != nullptr;
}

template <typename T>
T ListEnumerator<T>::Current() const {
    if (!current) {
        throw IndexOutOfRange(position, list->GetLength());
    }
    return current->data;
}

template <typename T>
void ListEnumerator<T>::Reset() {
    current = nullptr;
    position = -1;
}

//реализация методов LinkedList

template <typename T>
ListEnumerator<T>* LinkedList<T>::GetEnumeratorPtr() const {
    return new ListEnumerator<T>(this);
}

#endif