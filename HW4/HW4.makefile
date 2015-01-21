CC=g++
CFLAGS=-Wall -ggdb -std=c++11 -c

.PHONY: all
all: map

map: Map.o
	$(CC) -o map Map.o

Map.o: Map.cpp
	$(CC) $(CFLAGS) Map.cpp

