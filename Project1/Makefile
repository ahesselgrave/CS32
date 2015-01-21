CC=g++
CFLAGS=-std=c++0x -c -Wall -ggdb

.PHONY: all
all: project1

project1: main.o Zombie.o Player.o Arena.o Game.o utilities.o History.o
	$(CC) main.o Zombie.o Player.o Arena.o Game.o utilities.o History.o -o project1

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Zombie.o: Zombie.cpp
	$(CC) $(CFLAGS) Zombie.cpp

Player.o: Player.cpp
	$(CC) $(CFLAGS) Player.cpp

Arena.o: Arena.cpp
	$(CC) $(CFLAGS) Arena.cpp

Game.o: Game.cpp
	$(CC) $(CFLAGS) Game.cpp

utilities.o: utilities.cpp
	$(CC) $(CFLAGS) utilities.cpp

History.o: History.cpp
	$(CC) $(CFLAGS) History.cpp

clean:
	rm *.o
	rm project1
