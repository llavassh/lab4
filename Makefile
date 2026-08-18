CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
HEADERS = exceptions.h Option.h DynamicArray.h LinkedList.h IEnumerator.h \
          Sequence.h ArraySequence.h ImmutableArraySequence.h MutableArraySequence.h \
          ListSequence.h ImmutableListSequence.h MutableListSequence.h Stack.h Queue.h Deque.h Algorithms.h \
		  Generator.h Ordinal.h LazySequence.h

all: main tests

main: main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) main.cpp -o main

tests: tests.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) tests.cpp -o tests

test: tests
	./tests

run: main
	./main

clean:
	rm -f main tests

.PHONY: all test run clean