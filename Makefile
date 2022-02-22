CC = g++
CFLAGS = -Wall -Wextra -O3 -std=c++20
DEBUG = # -ggdb -O0 -pg
problem = accessible

all: build

build: main

orientable_dfa.o: orientable_dfa.cpp orientable_dfa.hpp
	$(CC) $(CFLAGS) $(DEBUG) -c $<

main.o: main.cpp
	$(CC) $(CFLAGS) $(DEBUG) -c $^

main: main.o orientable_dfa.o
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $^

.PHONY: clean

run: build
	./main $(problem)

clean:
	rm -f *.o *~ main
