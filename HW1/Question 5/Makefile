CC = g++
CFLAGS = -std=c++0x -c

.PHONY: all	
all: hw1q5

hw1q5: newMap.o testnewMap.o
	$(CC) newMap.o testnewMap.o -o hw1q5

testnewMap.o: testnewMap.cpp
	$(CC) $(CFLAGS) testnewMap.cpp

newMap.o: newMap.cpp
	$(CC) $(CFLAGS) newMap.cpp