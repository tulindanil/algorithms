CC = g++
CFLAGS = -g -c -std=c++14 
LDFLAGS = 
SOURCES = main.cpp binomial_queue.cpp binomial_tree.cpp 
OBJECTS = $(SOURCES:.cpp=.o)  
EXECUTABLE = 1_BINOM 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS)  $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

binomial_queue.hpp:
        

clean:
		rm *.o $(EXECUTABLE)

