#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H

#include <stdexcept>

#include "DynamicArray.h"
#include "Sequence.h"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
	DynamicArray<T>* items;

    //создает копию
    ArraySequence<T>* Clone() const {
        ArraySequence<T>* copy = CreateEmptySequence();
        for (int i = 0; i < GetLength(); i++) {
            copy->items->Add(items->Get(i));
        }
        return copy;
    }

    //создает последовательность типа T
    virtual ArraySequence<T> *CreateEmptySequence() const override = 0;

public:
    //конструкторы
    ArraySequence(): items(new DynamicArray<T>()) {}
    explicit ArraySequence(int size): items(new DynamicArray<T>(size)) {}
    ArraySequence(T* arr, int count): items(new DynamicArray<T>(arr, count)) {}
    ArraySequence(const ArraySequence<T> &other): items(new DynamicArray<T>(*other.items)) {}
    ArraySequence(const Sequence<T>* other) : items(new DynamicArray<T>()){
        for (int i = 0; i < other->GetLength(); i++) {
            items->Add(other->Get(i));
        }
    }

    //удаление
    virtual ~ArraySequence() {
        delete items;
    }

    //возвращает элемент по индексу
    const T& Get(int index) const override {
        if (index < 0 || index >= GetLength()) throw IndexOutOfRange(index, GetLength());
        return items->Get(index);
    }

    //возвращает первый элемент
    const T& GetFirst() const override {
        if (GetLength() == 0) throw EmptyContainer("ArraySequence");
        return items->Get(0);
    }

    //возвращает последний элемент
    const T& GetLast() const override {
        if (GetLength() == 0) throw EmptyContainer("ArraySequence");
        return items->Get(GetLength() - 1);
    }

    //возвращает длину
    int GetLength() const override {
        return items->GetSize();
    }

    ArraySequence<T>* Append(const T& item) const override {
        ArraySequence<T>* result = Instance();
        result->items->Add(item);
        return result;
    }
    
    ArraySequence<T>* Prepend(const T& item) const override {
        ArraySequence<T>* result = Instance();
        DynamicArray<T>* temp = new DynamicArray<T>(result->GetLength() + 1);
        temp->Set(0, item);
        for (int i = 0; i < result->GetLength(); i++) {
            temp->Set(i + 1, result->items->Get(i));
        }
        delete result->items;
        result->items = temp;
        return result;
    }
    
    ArraySequence<T>* InsertAt(int index, const T& item) const override {
        if (index < 0 || index > GetLength()) 
            throw IndexOutOfRange(index, GetLength());
        ArraySequence<T>* result = Instance();
        DynamicArray<T>* temp = new DynamicArray<T>(result->GetLength() + 1);
        for (int i = 0; i < index; i++) {
            temp->Set(i, result->items->Get(i));
        }
        temp->Set(index, item);
        for (int i = index; i < result->GetLength(); i++) {
            temp->Set(i + 1, result->items->Get(i));
        }
        delete result->items;
        result->items = temp;
        return result;
    }

    ArraySequence<T>* RemoveAt(int index) const override {
        ArraySequence<T>* result = Instance();
        if (result->GetLength() == 0)
            throw EmptyContainer("Sequence");
        if (index < 0 || index >= GetLength())
            throw IndexOutOfRange(index, GetLength());
        DynamicArray<T>* temp = new DynamicArray<T>();
        for (int i = 0; i < index; i++) {
            temp->Add(Get(i));
        }
        for (int i = index + 1; i < GetLength(); i++) {
            temp->Add(Get(i));
        }
        delete result->items;
        result->items = temp;
        return result;
    }

    //выделение подстроки
    ArraySequence<T>* GetSubSequence(int start, int end) const override {
        if (start < 0 || end >= GetLength() || end < start) throw IndexOutOfRange(start, GetLength());

        ArraySequence<T>* result = CreateEmptySequence();
        for (int i = 0; i < end - start + 1; i++) {
            ArraySequence *next = result->Append(Get(start + i));
            if (next != result)
                delete result;
            result = next;
        }
        return result;
    }

    //конкатенация
    ArraySequence<T>* Concat(const Sequence<T>* other) const override {
        ArraySequence<T>* result = Clone();
        for (int i = 0; i < other->GetLength(); i++)
            result->items->Add(other->Get(i));
        return result;
    }
    
    ArraySequence<T>* Slice(int start, int count, const Sequence<T>* insertSeq = nullptr) const override {
        ArraySequence<T>* result = Instance();
        
        if (start < 0) start = result->GetLength() + start;
        if (start < 0 || start > result->GetLength()) 
            throw IndexOutOfRange(start, result->GetLength());
        if (count < 0) 
            throw std::invalid_argument("Negative count");
        if (start + count > result->GetLength()) 
            throw std::out_of_range("Slice extends beyond sequence bounds");
        
        DynamicArray<T>* temp = new DynamicArray<T>();
        for (int i = 0; i < start; i++) {
            temp->Add(result->items->Get(i));
        }
        if (insertSeq) {
            for (int i = 0; i < insertSeq->GetLength(); i++) {
                temp->Add(insertSeq->Get(i));
            }
        }
        for (int i = start + count; i < result->GetLength(); i++) {
            temp->Add(result->items->Get(i));
        }
        delete result->items;
        result->items = temp;
        return result;
    }

    ArrayEnumerator<T>* GetEnumerator() const override {
        return new ArrayEnumerator<T>(items);
    }

    virtual ArraySequence<T> *Instance() const override = 0;
    
};

#endif