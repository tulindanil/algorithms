CC = g++
CFLAGS = -g -c -Wc++11-extensions -std=c++14 -I../2015_09_04/0_GRAPH 
LDFLAGS = 
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
	EXECUTABLE = Floyd 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		    $(CC) $(CFLAGS) $< -o $@

clean:
		rm *.o $(EXECUTABLE)

