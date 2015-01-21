CC = g++

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean


all: orakle

%.o: %.cpp $(HEADERS)
	$(CC) -c -std=c++11 $< -o $@

orakle: $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f *.o
	rm -f orakle