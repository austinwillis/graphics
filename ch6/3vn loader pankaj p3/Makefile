FILE       = project3.cpp 
EXECUTABLE = project3
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lglut -lGLU -lGL
CC         = g++ -Wall -std=c++11
OBJECTS		 = camera.o SDL.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS)

$.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
