
all:
	g++ -std=c++11 main.cpp simulator.cpp buddy_allocator.cpp page_reclamation.cpp lru.cpp

run: all
	./a.out < A0204752L-assign4-input.dat

clean:
	rm a.out
