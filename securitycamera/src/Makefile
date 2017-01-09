CC=g++
LINKFLAGS=-O2 `pkg-config --cflags --libs opencv` -lpthread
SOURCES=v4l2camera.cpp upload.cpp capture.cpp motiondetect.cpp main.cpp led.cpp deletion.cpp filelog.cpp
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
