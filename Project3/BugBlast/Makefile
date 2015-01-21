CC = g++
LIBS = -lglut -lGL -lGLU

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean


all: BugBlast

%.o: %.cpp $(HEADERS)
	$(CC) -c -std=c++11 $< -o $@

BugBlast: $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f BugBlast
