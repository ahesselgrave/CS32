CC = g++
CFLAGS = -std=c++0x -ggdb -c -Wall

.PHONY: all	
all: hw1q4

hw1q4: Map.o testSalaryMap.o SalaryMap.o
	$(CC) Map.o testSalaryMap.o SalaryMap.o -o hw1q4

SalaryMap.o: SalaryMap.cpp
	$(CC) $(CFLAGS) SalaryMap.cpp

testSalaryMap.o: testSalaryMap.cpp
	$(CC) $(CFLAGS) testSalaryMap.cpp

Map.o: Map.cpp
	$(CC) $(CFLAGS) Map.cpp