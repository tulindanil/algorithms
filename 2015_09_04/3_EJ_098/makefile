CC = g++
CFLAGS = -std=c++14 -g -c  -I../0_GRAPH 
LDFLAGS = 
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
	EXECUTABLE = 3_EJ_098 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		    $(CC) $(CFLAGS) $< -o $@

clean:
		rm *.o $(EXECUTABLE)

