CC=g++
CFLAGS=-Wall -ggdb -std=c++11 -c

.PHONY: all
all: tradingtown

tradingtown: tradingtown.o
	$(CC) -o tradingtown trading.o

trading.o: trading.cpp
	$(CC) $(CFLAGS) trading.cpp



