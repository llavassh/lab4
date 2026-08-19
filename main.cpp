#include "LazySequence.h"
#include <iostream>

using namespace std;

static int TwoPlus(Sequence<int>* seq) {
    return seq->Get(0) + seq->Get(1);
}

static int ThreePlus(Sequence<int>* seq) {
    return seq->Get(0) + seq->Get(1) + seq->Get(2);
}

static int Double(Sequence<int> *seq) {
    return seq -> Get(0) * 2; 
}

static int dbl(const int &x) {
    return x * 2;
}

static int sqr(const int& x) {
    return x * x;
}

static int neg(const int& x) {
    return -x;
}

static int add10(const int& x) {
    return x + 10;
}

int *ReadArray(int count) {
    if (count <= 0)
        return nullptr;
    int *arr = new int[count];
    cout << "Enter " << count << " integers: " << endl;
    for (int i = 0; i < count; i++) {
        cin >> arr[i];
    }
    return arr;
}

Ordinal ReadOrdinal() {
    int omegaCount, finiteSize;
    cout << "Enter omega count: " << endl;
    cin >> omegaCount;
    cout << "Enter finite size: " << endl;
    cin >> finiteSize;
    return Ordinal(omegaCount, finiteSize);
}

LazySequence<int>* CreateLazySequence() {
    LazySequence<int> *lseq = nullptr;
    cout << "1. Create finite sequence" << endl;
    cout << "2. Create infinite sequence" << endl;
    cout << "0. Exit" << endl;
    int choice1;
            
    cin >> choice1;
    switch(choice1) {
        case 1: {
            int count;
            cout << "Enter the sequence length" << endl;
            cin >> count;
            int* arr = ReadArray(count);
            lseq = new LazySequence<int>(arr, count);
            delete[] arr;
            break;
        }
        case 2: {
            int choice2;

            cout << "Choose a generation rule: " << endl;
            cout << "1. Sum of previous two elements" << endl;
            cout << "2. Sum of the previous three elements" << endl;
            cout << "3. Double the previous element" << endl;
            cout << "0. Exit" << endl;
            cout << "Your choice: " << endl;
            cin >> choice2;
            switch(choice2) {
                case 1: {
                    cout << "Enter two elements: "<< endl;
                    int *arr = ReadArray(2);
                    MutableArraySequence<int> seq(arr, 2);
                    lseq = new LazySequence<int>(TwoPlus, &seq);
                    delete[] arr;
                    break;
                }
                case 2: {
                    cout << "Enter three elements: " << endl;
                    int *arr = ReadArray(3);
                    MutableArraySequence<int> seq(arr, 3);
                    lseq = new LazySequence<int>(ThreePlus, &seq);
                    delete[] arr;
                    break;
                }
                case 3: {
                    cout << "Enter element" << endl;
                    int *arr = ReadArray(1);
                    MutableArraySequence<int> seq(arr, 1);
                    lseq = new LazySequence<int>(Double, &seq);
                    delete[] arr;
                    break;
                }
                case 0:
                    break;
                default: {
                    cout << "Unknown command" << endl;
                    break;
                }
            }
            break;
        }
        case 0: {
            break;
        }
        default: {
            cout << "Unknown command" << endl;
            break;
        }
    }
    return lseq;
}

void UI() {
    int choice;
    LazySequence<int> *lseq = nullptr;
    do {
        cout << "---LAZY SEQUENCE---" << endl;
        cout << "1. Create Lazy Sequence" << endl;
        cout << "2. Get element" << endl;
        cout << "3. Insert element" << endl;
        cout << "4. Append element" << endl;
        cout << "5. Prepend element" << endl;
        cout << "6. Remove element" << endl;
        cout << "7. Get Subsequence" << endl;
        cout << "8. Concat" << endl;
        cout << "9. Map" << endl;
        cout << "10. Show sequence" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;
        if (lseq == nullptr && choice != 1 && choice != 0) {
            cout << "Create a sequence first" << endl;
            continue;
        }
        switch(choice) {
            case 1: {
                LazySequence<int>* newLseq = CreateLazySequence();
                if (newLseq != nullptr) {
                    delete lseq;
                    lseq = newLseq;
                }
                break;
            }
            case 2: {
                cout << "Enter index: " << endl;
                Ordinal index = ReadOrdinal();
                cout << "Element: " << lseq->Get(index) << endl;
                break;
            }
            case 3: {
                int value;
                cout << "Enter value: " << endl;
                cin >> value;
                cout << "Enter index: " << endl;
                Ordinal index = ReadOrdinal();
                LazySequence<int> *newLseq = lseq->InsertAt(index, value);
                delete lseq;
                lseq = newLseq;
                break;
            }
            case 4: {
                int value;
                cout << "Enter value: " << endl;
                cin >> value;
                LazySequence<int> *newLseq = lseq->Append(value);
                delete lseq;
                lseq = newLseq;
                break;
            }
            case 5: {
                int value;
                cout << "Enter value: " << endl;
                cin >> value;
                LazySequence<int> *newLseq = lseq->Prepend(value);
                delete lseq;
                lseq = newLseq;
                break;
            }
            case 6: {
                cout << "Enter index: " << endl;
                Ordinal index = ReadOrdinal();
                LazySequence<int> *newLseq = lseq->RemoveAt(index);
                delete lseq;
                lseq = newLseq;
                break;
            }
            case 7: {
                cout << "Enter start: " << endl;
                Ordinal start = ReadOrdinal();
                cout << "Enter end: " << endl;
                Ordinal end = ReadOrdinal();
                LazySequence<int> *newLseq = lseq->GetSubSequence(start, end);
                delete lseq;
                lseq = newLseq;
                break;
            }
            case 8: {
                LazySequence<int> *second = CreateLazySequence();
                if (second == nullptr)
                    break;
                LazySequence<int> *newLseq = lseq->Concat(*second);
                delete lseq;
                delete second;
                lseq = newLseq;
                break;
            }
            case 9: {
                int choice1;
                cout << "Choose a map function: " << endl;
                cout << "1. Multyply by 2" << endl;
                cout << "2. Square" << endl;
                cout << "3. Change sign" << endl;
                cout << "4. Add 10" << endl;
                cout << "0. Back" << endl;
                cout << "Your choice: ";
                cin >> choice1;
                switch(choice1) {
                    case 1: {
                        LazySequence<int>* newLseq = lseq->Map(dbl);
                        delete lseq;
                        lseq = newLseq;
                        break;
                    }
                    case 2: {
                        LazySequence<int>* newLseq = lseq->Map(sqr);
                        delete lseq;
                        lseq = newLseq;
                        break;
                    }
                    case 3: {
                        LazySequence<int>* newLseq = lseq->Map(neg);
                        delete lseq;
                        lseq = newLseq;
                        break;
                    }
                    case 4: {
                        LazySequence<int>* newLseq = lseq->Map(add10);
                        delete lseq;
                        lseq = newLseq;
                        break;
                    }
                    case 0: {
                        break;
                    }
                    default: {
                        cout << "Unknown command";
                    }
                }
                break;
            }
            case 10: {
                if (lseq->GetLength().GetOmegaCount() == 0) {
                    for (int i = 0; i < lseq->GetLength().GetFiniteSize(); i++) {
                        cout << lseq->Get(Ordinal(0, i)) << " ";
                    }
                }
                else {
                    for (int omega = 0; omega < lseq->GetLength().GetOmegaCount(); omega++) {
                        for (int i = 0; i < 10; i++) {
                            cout << lseq->Get(Ordinal(omega, i)) << " ";
                        }
                        cout << "...";
                    }
                    for (int i = 0; i < lseq->GetLength().GetFiniteSize(); i++) {
                        cout << lseq->Get(Ordinal(lseq->GetLength().GetOmegaCount(), i)) << " ";
                    }
                }
                break;
            }
            case 0: {
                return;
            }
            default: {
                cout << "Unknown command" << endl;
                break;
            }
        }
    } while (choice != 0);
    delete lseq;
}

int main() {
    UI();
    return 0;
}