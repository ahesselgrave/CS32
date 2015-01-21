CC=g++
CLAGS=-ggdb -std=c++11 -c -Wall

.PHONY: all
all: project2

project2: Map.o testDLLmap.o
	$(CC) Map.o testDLLmap.o -o project2

Map.o: Map.cpp
	$(CC) $(CFLAGS) Map.cpp

testDLLmap.o: testDLLmap.cpp
	$(CC) $(CFLAGS) testDLLmap.cpp