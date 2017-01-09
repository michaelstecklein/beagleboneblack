CC=g++
LINKFLAGS=-O2 `pkg-config --cflags --libs opencv` -lpthread
SOURCES=motiondetection_example.cpp
HEADERS=*.hpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=run


%.o: %.cpp $(HEADERS)
	$(CC) -c $<

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LINKFLAGS) -o $@ $^


.PHONY: clean

clean:
	rm *.o
	rm $(EXECUTABLE)
