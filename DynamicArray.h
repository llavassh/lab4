#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include "Exceptions.h"

template <typename T>
class DynamicArray {
private:
    T* data;
    int size;
    int capacity;

public:
    //создает пустой динамический массив
    DynamicArray(): data(nullptr), size(0), capacity(0) {}

    //создает динамический массив заданного размера
    explicit DynamicArray(int size): size(size), capacity(size) {
        if (size < 0) 
            throw std::invalid_argument("Negative size");
        data = (size > 0) ? new T[size] : nullptr; 
    }

    //создает динамический массив и копирует в него элементы из исходного обычного массива
    DynamicArray(T* items, int count): size(count), capacity(count) {
        if (count < 0) 
            throw std::invalid_argument("Negative count");
        if (count > 0) {
            data = new T[count];
            for (int i = 0; i < count; i++) {
                data[i] = items[i];
            }
        }
        else {
            data = nullptr;
        }
    }

    //создает  массив и копирует в него элементы из другого динамического массива
    DynamicArray(const DynamicArray<T> &other): size(other.size), capacity(other.capacity) {
        if (capacity > 0) {
            data = new T[capacity];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        else {
            data = nullptr;
        }
    }

    //уничтожение массива
    ~DynamicArray() {
        delete[] data;
    }

    //присвоить один динамический массив к другому
    DynamicArray<T>& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            if (capacity > 0) {
                data = new T[capacity];
                for (int i = 0; i < size; i++) {
                    data[i] = other.data[i];
                }
            }
            else {
                data = nullptr;
            }
        }
        return *this;
    }

    //возвращает элемент по заданному индексу
    const T& Get(int index) const {
        if (index < 0 || index >= size) throw IndexOutOfRange(index, size);
        
        return data[index];
    }

    //устанавливает значение элемента по индексу
    void Set(int index, const T &value) {
        if (index < 0 || index >= size)
            throw IndexOutOfRange(index, size);

        data[index] = value;
    }

    //возвращает размер
    int GetSize() const {
        return size;
    }

    int GetCapacity() const {
        return capacity;
    }
    //изменяет размер массива
    void Resize(int newSize) {
        if (newSize < 0)
            throw std::invalid_argument("Negative size");
        if (newSize > capacity) {
            int newCapacity = (capacity == 0) ? 1 : capacity;
            while (newSize > newCapacity) {
                newCapacity *= 2;
            }
            T *newData = new T[newCapacity];
            for (int i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }
        size = newSize;
    }

    //быстрая вставка в конец
    void Add(const T &item) {
        Resize(size + 1);
        data[size - 1] = item;
    }

    //позволяет обращаться к элементам динамического массива, как к обычному массиву (для изменения)
    T &operator[](int index) {
        if (index < 0 || index >= size) throw IndexOutOfRange(index, size);
        return data[index];
    }

    //позволяет обращаться к элементам динамического массива, как к обычному массиву (для чтения)
    const T &operator[](int index) const {
        if (index < 0 || index >= size) throw IndexOutOfRange(index, size);
        return data[index];
    }

};
#endif