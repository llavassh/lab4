#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Exceptions.h"

template <typename T> class ListEnumerator;

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T &value): data(value), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int length;

public:
    //создание пустого связного списка
    LinkedList(): head(nullptr), tail(nullptr), length(0) {}

    //создание связного списка и заполнение его элементами обычного массива
    LinkedList(T* items, int count) : LinkedList() {
        for (int i = 0; i < count; i++)
            Append(items[i]);
    }

    //создает копию исходного списка
    LinkedList(const LinkedList<T> &other) : LinkedList() {
        Node* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }

    //удаление списка
    ~LinkedList() {
        Clear();
    }

    //присвоить один список другому 
    LinkedList<T> &operator=(const LinkedList<T> &other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        Node* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }

        return *this;
    }

    //возвращает первый элемент
    const T& GetFirst() const {
        if (length == 0) throw EmptyContainer("LinkedList");

        return head->data;
    }

    //возвращает последний элемент
    const T& GetLast() const {
        if (length == 0) throw EmptyContainer("LinkedList");

        return tail->data;
    }

    //возвращает элемет по индексу
    const T& Get(int index) const {
        if (index < 0 || index >= length) throw IndexOutOfRange(index, length);

        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    //возвращает список элементов с startindex до endindex
    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= length || startIndex > endIndex) throw IndexOutOfRange(startIndex, length);

        LinkedList<T>* result = new LinkedList<T>();

        Node* current = head;
        for (int i = 0; i < startIndex; i++) {
            current = current->next;
        }

        for (int i = startIndex; i <= endIndex; i++) {
            result->Append(current->data);
            current = current->next;
        }

        return result;
    }

    //возвращает размер
    int GetLength() const {
        return length;
    }

    //добавить элемент в конец списка
    void Append(T const &item) {
        Node* newNode = new Node(item);
        if (length == 0) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
    }

    //добавить элемент в начало списка
    void Prepend(T const &item) {
        Node* newNode = new Node(item);
        if (length == 0) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
        
        length++;
    }


    //вставляет элемент по индексу
    void InsertAt(int index, T const &item) {
        if (index < 0 || index > length) throw IndexOutOfRange(index, length);

        if (index == 0) {
            Prepend(item);
        }

        else if (index == length) {
            Append(item);
        }

        else {
            Node* current = head;
            for (int i = 0; i < index - 1; i++) {
                current = current->next;
            }
            Node* newNode = new Node(item);
            newNode->next = current->next;
            current->next = newNode;
            length++; 
        }
    }

    //конкатенация двух списков
    LinkedList<T>* Concat(const LinkedList<T> &other) {
        LinkedList<T>* result = new LinkedList();

        Node* current = head;
        while(current) {
            result->Append(current->data);
            current = current->next;
        }

        current = other.head;
        while (current) {
            result->Append(current->data);
            current = current->next;
        }

        return result;
    }

    //очищает список
    void Clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp; 
        }

        tail = nullptr;
        length = 0;
    }

    //операторы сравнения двух списков
    bool operator==(const LinkedList<T> &other) const {
        if (length != other.length) return false;

        Node* curr1 = head;
        Node* curr2 = other.head;
        while (curr1) {
            if (curr1->data != curr2->data) return false;
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
        return true;
    }
    
    bool operator!=(const LinkedList<T> &other) const {
        return !(*this == other);
    }

    template <typename U>
    friend class ListEnumerator;

    ListEnumerator<T>* GetEnumeratorPtr() const;

};

#endif