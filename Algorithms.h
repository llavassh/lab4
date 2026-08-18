#ifndef ALGORITHMS_H
#define ALGORITHMS_H
 
#include "MutableArraySequence.h"
#include "Sequence.h"
#include <stdexcept>
 
//инверсии
template <typename T>
int CountInversions(const Sequence<T>* seq) {
    int count = 0;
    int n = seq->GetLength();
    for (int i = 0; i < n - 1; ++i)
        for (int j = i + 1; j < n; ++j)
            if (seq->Get(i) > seq->Get(j))
                ++count;
    return count;
}

//факториал
MutableArraySequence<long long>* Factorial(int count) {
    if (count < 0) throw std::invalid_argument("Negative count");
    auto* seq = new MutableArraySequence<long long>();
    seq->Append(1);
    for (int i = 1; i <= count; i++) {
        seq->Append(seq->Get(i - 1) * i);
    }
    return seq;
}
 
//фибоначчи
MutableArraySequence<long long>* Fibonacci(int count) {
    if (count < 1) throw std::invalid_argument("n must be >= 1");
    auto* seq = new MutableArraySequence<long long>();
    seq->Append(0LL);
    if (count == 1) return seq;
    seq->Append(1LL);
    for (int i = 2; i < count; ++i)
        seq->Append(seq->Get(i - 1) + seq->Get(i - 2));
    return seq;
}

//range
MutableArraySequence<int>* Range(int l, int h) {
    if (l > h) throw std::invalid_argument("Range: l > h");
    auto* seq = new MutableArraySequence<int>();
    for (int i = l; i <= h; ++i)
        seq->Append(i);
    return seq;
}
 
//выделение простых чисел
static bool isPrime(const int& x) {
    if (x < 2) return false;
    for (int d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

MutableArraySequence<int>* Primes(int n) {
    if (n < 2) return new MutableArraySequence<int>();
    MutableArraySequence<int>* r = Range(2, n);
    Sequence<int>* filtered = r->Where(isPrime);
    delete r;
    return static_cast<MutableArraySequence<int>*>(filtered);
}
 
#endif 