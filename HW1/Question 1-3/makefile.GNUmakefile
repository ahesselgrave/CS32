CC = g++
CFLAGS = -std=c++0x -ggdb -c -Wall

.PHONY: all	
all: hw1q3

hw1q3: Map.h Map.o testmap.o
	$(CC) Map.h Map.o testmap.o -o hw1q3

Map.o: Map.cpp
	$(CC) $(CFLAGS) Map.cpp

testmap.o: testmap.cpp
	$(CC) $(CFLAGS) testmap.cpp

