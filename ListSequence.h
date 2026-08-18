#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "LinkedList.h"
#include "Sequence.h"
#include "IEnumerator.h"

template <typename T>
class ListSequence: public Sequence<T> {
protected: 
    LinkedList<T>* list;

    //создать копию 
    ListSequence<T>* Clone() const {
        ListSequence<T>* copy = CreateEmptySequence();
        ListEnumerator<T>* enumerator = list->GetEnumeratorPtr();
        while (enumerator->MoveNext()) {
            copy->list->Append(enumerator->Current());
        }
        delete enumerator;
        return copy;
    }

    //создает последовательность типа T
    virtual ListSequence<T> *CreateEmptySequence() const override = 0;

public: 
    ListSequence(): list(new LinkedList<T>()) {}
    ListSequence(T *arr, int count): list(new LinkedList<T>(arr, count)) {}
    ListSequence(const LinkedList<T>& other): list(new LinkedList<T>(other)) {}
    ListSequence(const ListSequence<T>& other): list(new LinkedList<T>(*other.list)) {}  

    //удалить
    virtual ~ListSequence() {
        delete list;
    }

    //возвращает элемент по индексу
    const T& Get(int index) const override {
        return list->Get(index);
    }

    //возвращает первый элемент
    const T& GetFirst() const override {
        return list->GetFirst();
    }

    //возвращает последний элемент
    const T& GetLast() const override {
        return list->GetLast();
    }

    //возвращет длину
    int GetLength() const override {
        return list->GetLength();
    }

    ListSequence<T>* Append(const T& item) const override {
        ListSequence<T>* result = Instance();
        result->list->Append(item);
        return result;
    }
    
    ListSequence<T>* Prepend(const T& item) const override {
        ListSequence<T>* result = Instance();
        result->list->Prepend(item);
        return result;
    }
    
    ListSequence<T>* InsertAt(int index, const T& item) const override {
        if (index < 0 || index > GetLength()) 
            throw IndexOutOfRange(index, GetLength());
        ListSequence<T>* result = Instance();
        result->list->InsertAt(index, item);
        return result;
    }

    ListSequence<T>* RemoveAt(int index) const override {
        ListSequence<T>* result = Instance();
        if (result->GetLength() == 0)
            throw EmptyContainer("Sequence");
        if (index < 0 || index >= GetLength())
            throw IndexOutOfRange(index, GetLength());
        LinkedList<T>* temp = new LinkedList<T>();
        for (int i = 0; i < index; i++) {
            temp->Append(Get(i));
        }
        for (int i = index + 1; i < GetLength(); i++) {
            temp->Append(Get(i));
        }
        delete result->list;
        result->list = temp;
        return result;
    }


    //возвращает подстроку
    ListSequence<T>* GetSubSequence(int start, int end) const override {
        if (start < 0 || end >= GetLength() || end < start) throw IndexOutOfRange(start, GetLength());

        ListSequence<T>* result = CreateEmptySequence();
        ListEnumerator<T>* enumerator = list->GetEnumeratorPtr();
        int i = 0;
        while (enumerator->MoveNext()) {
            if (i > end) break;
            if (i >= start) {
                result->list->Append(enumerator->Current());
            }
            i++;
        }
        delete enumerator;
        return result;
    }

    //конкатенация
    ListSequence<T>* Concat(const Sequence<T>* other) const override {
        ListSequence<T>* result = Clone();
        IEnumerator<T>* enumerator = other->GetEnumerator();
        while (enumerator->MoveNext()) {
            result->list->Append(enumerator->Current());
        }
        delete enumerator;
        return result;
    }

    ListSequence<T>* Slice(int start, int count, const Sequence<T>* insertSeq) const override {
        ListSequence<T>* result = Instance();
        
        if (start < 0) start = result->GetLength() + start;
        if (start < 0 || start > result->GetLength()) 
            throw IndexOutOfRange(start, result->GetLength());
        if (count < 0) 
            throw std::invalid_argument("Negative count");
        if (start + count > result->GetLength()) 
            throw std::out_of_range("Slice extends beyond sequence bounds");
        
        LinkedList<T>* temp = new LinkedList<T>();
        
        for (int i = 0; i < start; i++) {
            temp->Append(result->list->Get(i));
        }
        
        if (insertSeq) {
            for (int i = 0; i < insertSeq->GetLength(); i++) {
                temp->Append(insertSeq->Get(i));
            }
        }
        
        for (int i = start + count; i < result->GetLength(); i++) {
            temp->Append(result->list->Get(i));
        }
        
        delete result->list;
        result->list = temp;
        return result;
    }

    //возвращает итератор для обхода последовательности
    ListEnumerator<T>* GetEnumerator() const override {
        return new ListEnumerator<T>(list);
    }

    ListSequence<T> *Instance() const override = 0;
};

#endif