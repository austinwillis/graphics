FILE       = main.cpp 
EXECUTABLE = Camera
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
CC         = g++ -Wall -std=c++11
DEPS = camera.h
OBJECTS = main.o camera.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS)

$.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
