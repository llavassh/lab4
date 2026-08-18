#include <cassert>
#include "LazySequence.h"
#include "Generator.h"

static int Fibonacci(Sequence<int>* seq) {
    return seq->Get(0) + seq->Get(1);
}

static int ThreePlus(Sequence<int>* seq) {
    return seq->Get(0) + seq->Get(1) + seq->Get(2);
}

static int dbl(const int &x) {
    return x * 2;
}

static int sqr(const int& x) {
    return x * x;
}

void testsLazySequence() {
    auto empty = LazySequence<int>();
    bool thrown = false;
    try {
        empty.GetFirst();
    } catch(EmptyContainer&) {
        thrown = true;
    }
    assert(thrown);
    bool thrown0 = false;
    try {
        empty.GetLast();
    } catch(EmptyContainer&) {
        thrown0 = true;
    }
    assert(thrown0);

    int arr1[] = {1, 2, 3, 4, 5};
    auto lseq1 = LazySequence<int>(arr1, 5);
    assert(lseq1.Get(Ordinal(0, 0)) == 1);
    assert(lseq1.Get(Ordinal(0, 2)) == 3);
    assert(lseq1.GetFirst() == 1);
    assert(lseq1.GetMaterializedCount() == 3);
    assert(lseq1.GetLast() == 5);
    bool thrown1 = false;
    try {
        lseq1.Get(Ordinal(0, 100));
    } catch (OrdinalIndexOutOfRange&) {
        thrown1 = true;
    }
    assert(thrown1);
    bool thrown2 = false;
    try {
        lseq1.Get(Ordinal(1, 0));
    } catch (OrdinalIndexOutOfRange&) {
        thrown2 = true;
    }
    assert(thrown2);
    assert(lseq1.GetFirst() == 1);
    assert(lseq1.GetLast() == 5);
    assert(lseq1.GetLength() == Ordinal(0, 5)); 

    int arr2[] = {1, 1};
    auto seq2 = MutableArraySequence<int>(arr2, 2);
    auto lseq2 = LazySequence<int>(Fibonacci, &seq2);
    assert(lseq2.Get(Ordinal(0, 0)) == 1);
    assert(lseq2.Get(Ordinal(0, 1)) == 1);
    assert(lseq2.Get(Ordinal(0, 2)) == 2);
    assert(lseq2.GetFirst() == 1);
    assert(lseq2.GetMaterializedCount() == 3);
    assert(lseq2.Get(Ordinal(0, 7)) == 21);
    assert(lseq2.Get(Ordinal(0, 4)) == 5);
    assert(lseq2.GetMaterializedCount() == 8);
    assert(lseq2.GetFirst() == 1);
    bool thrown3 = false;
    try {
        lseq2.GetLast();
    } catch(std::logic_error&) {
        thrown3 = true;
    }
    assert(thrown3);
    assert(lseq2.GetLength() == Ordinal(1, 0));

    auto lseq3 = lseq2;
    assert(lseq3.GetLength() == Ordinal(1, 0));
    assert(lseq3.GetMaterializedCount() == 8);
    lseq3.Get(Ordinal(0, 9));
    assert(lseq3.GetMaterializedCount() == 10);
    assert(lseq2.GetMaterializedCount() == 8);
    bool thrown4 = false;
    try {
        lseq3.GetLast();
    } catch (std::logic_error&) {
        thrown4 = true;
    }
    assert(thrown4);
}

void testsOperations() {
    int arr2[] = {1, 1};
    auto seq2 = MutableArraySequence<int>(arr2, 2);
    auto lseq2 = LazySequence<int>(Fibonacci, &seq2);

    auto lseq3 = lseq2.Prepend(32);
    assert(lseq3->GetFirst() == 32);
    assert(lseq3->Get(Ordinal(0, 1)) == 1);

    auto lseq4 = lseq2.Append(15);
    assert(lseq4->GetLast() == 15);
    assert(lseq4->GetLength() == Ordinal(1, 1));

    auto lseq5 = lseq4->InsertAt(Ordinal(0, 3), 12);
    assert(lseq5->Get(Ordinal(0, 1)) == 1);
    assert(lseq5->Get(Ordinal(0, 3)) == 12);
    assert(lseq5->Get(Ordinal(0, 8)) == 21);
    assert(lseq5->Get(Ordinal(1, 0)) == 15);
    assert(lseq4->GetLength() == Ordinal(1, 1));

    auto lseq6 = lseq5->InsertAt(Ordinal(1, 0), 25);
    assert(lseq6->Get(Ordinal(0, 2)) == 2);
    assert(lseq6->Get(Ordinal(1, 0)) == 25);
    assert(lseq6->Get(Ordinal(1, 1)) == 15);
    assert(lseq6->GetLength() == Ordinal(1, 3));

    int arr1[] = {1, 2, 3, 4, 5};
    auto lseq0 = LazySequence<int>(arr1, 5);
    bool thrown1 = false;
    try {
        lseq0.InsertAt(Ordinal(0, 6), 100);
    } catch (OrdinalIndexOutOfRange&) {
        thrown1 = true;
    }
    assert(thrown1);
    auto lseq10 = lseq0.InsertAt(Ordinal(0, 5), 100);
    assert(lseq10->GetLength() == Ordinal(0, 6));
    assert(lseq10->GetLast() == 100);

    auto lseq7 = lseq2.RemoveAt(Ordinal(0, 4));
    assert(lseq7->Get(Ordinal(0, 3)) == 3);
    assert(lseq7->Get(Ordinal(0, 4)) == 8);
    auto lseq8 = lseq6->RemoveAt(Ordinal(0, 0));
    assert(lseq8->Get(Ordinal(1, 1)) == 15);
    auto lseq9 = lseq6->RemoveAt(Ordinal(1, 0));
    assert(lseq9->Get(Ordinal(1, 0)) == 15);
    assert(lseq9->GetLength() == Ordinal(1, 2));
    bool thrown2 = false;
    try {
        lseq0.RemoveAt(Ordinal(0, 6));
    } catch (OrdinalIndexOutOfRange&) {
        thrown2 = true;
    }
    assert(thrown2);

    delete lseq3;
    delete lseq4;
    delete lseq5;
    delete lseq6;
    delete lseq7;
    delete lseq8;
    delete lseq9;
    delete lseq10;
}

void testsAlgorithms() {
    int arr1[] = {1, 1};
    auto seq1 = MutableArraySequence<int>(arr1, 2);
    auto lseq1 = LazySequence<int>(Fibonacci, &seq1);
    int arr2[] = {1, 1, 1};
    auto seq2 = MutableArraySequence<int>(arr2, 3);
    auto lseq2 = LazySequence<int>(ThreePlus, &seq2);
    int arr3[] = {1, 2, 3, 4, 5};
    auto lseq3 = LazySequence<int>(arr3, 5);
    int arr4[] = {100, 200, 300};
    auto lseq4 = LazySequence<int>(arr4, 3);

    auto concat1 = lseq3.Concat(lseq4);
    assert(concat1->GetLength() == Ordinal(0, 8));
    assert(concat1->Get(Ordinal(0, 4)) == 5);
    assert(concat1->Get(Ordinal(0, 5)) == 100);
    assert(concat1->GetLast() == 300);
    auto concat2 = lseq3.Concat(lseq1);
    assert(concat2->GetLength() == Ordinal(1, 0));
    assert(concat2->Get(Ordinal(0, 4)) == 5);
    assert(concat2->Get(Ordinal(0, 5)) == 1);
    auto concat3 = lseq1.Concat(lseq4);
    assert(concat3->GetLength() == Ordinal(1, 3));
    assert(concat3->Get(Ordinal(1, 0)) == 100);
    assert(concat3->Get(Ordinal(1, 1)) == 200);
    assert(concat3->Get(Ordinal(0, 0)) == 1);
    auto concat4 = lseq1.Concat(lseq2);
    assert(concat4->GetLength() == Ordinal(2, 0));
    assert(concat4->Get(Ordinal(1, 0)) == 1);
    assert(concat4->Get(Ordinal(0, 0)) == 1);
    assert(concat4->Get(Ordinal(1, 1)) == 1);
    assert(concat4->Get(Ordinal(1, 3)) == 3);

    auto sub1 = lseq3.GetSubSequence(Ordinal(0, 1), Ordinal(0, 3));
    assert(sub1->Get(Ordinal(0, 0)) == 2);
    assert(sub1->Get(Ordinal(0, 1)) == 3);
    assert(sub1->Get(Ordinal(0, 2)) == 4);
    assert(sub1->GetLength() == Ordinal(0, 3));
    auto sub2 = concat4->GetSubSequence(Ordinal(0, 1), Ordinal(1, 3));
    assert(sub2->GetFirst() == 1);
    assert(sub2->GetLast() == 3);
    assert(sub2->GetLength() == Ordinal(1, 4));
    auto sub3 = lseq3.GetSubSequence(Ordinal(0, 0), Ordinal(0, 4));
    assert(sub3->GetFirst() == 1);
    assert(sub3->GetLast() == 5);
    assert(sub3->GetLength() == Ordinal(0, 5));
    auto sub4 = lseq3.GetSubSequence(Ordinal(0, 2), Ordinal(0, 2));
    assert(sub4->GetFirst() == 3);
    assert(sub4->GetLast() == 3);
    assert(sub4->GetLength() == Ordinal(0, 1));
    bool thrown1 = false;
    try {
        lseq3.GetSubSequence(Ordinal(0, 3) , Ordinal(1, 3));
    } catch(OrdinalIndexOutOfRange&) {
        thrown1 = true;
    }
    assert(thrown1);
    bool thrown2 = false;
    try {
        lseq3.GetSubSequence(Ordinal(0, 4) , Ordinal(0, 1));
    } catch(OrdinalIndexOutOfRange&) {
        thrown2 = true;
    }
    assert(thrown2);

    auto map1 = lseq3.Map(dbl);
    assert(map1->Get(Ordinal(0, 0)) == 2);
    assert(map1->Get(Ordinal(0, 1)) == 4);
    assert(map1->Get(Ordinal(0, 2)) == 6);
    assert(map1->Get(Ordinal(0, 3)) == 8);
    assert(map1->Get(Ordinal(0, 4)) == 10);
    auto map2 = lseq1.Map(sqr);
    assert(map2->Get(Ordinal(0, 0)) == 1);
    assert(map2->Get(Ordinal(0, 1)) == 1);
    assert(map2->Get(Ordinal(0, 2)) == 4);
    assert(map2->Get(Ordinal(0, 3)) == 9);
    assert(map2->Get(Ordinal(0, 4)) == 25);
    auto map3 = concat3->Map(dbl);
    assert(map3->Get(Ordinal(1, 0)) == 200);

    delete concat1;
    delete concat2;
    delete concat3;
    delete concat4;
    delete sub1;
    delete sub2;
    delete sub3;
    delete sub4;
    delete map1;
    delete map2;
    delete map3;
}

int main() {
    testsLazySequence();
    testsOperations();
    testsAlgorithms();
    return 0;
}