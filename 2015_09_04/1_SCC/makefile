CC = g++
CFLAGS = -g -c -Wc++11-extensions -std=c++14 -I../0_GRAPH 
LDFLAGS = 
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
	EXECUTABLE = test 

test: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		    $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		    $(CC) $(CFLAGS) $< -o $@

clean:
		rm *.o $(EXECUTABLE)

