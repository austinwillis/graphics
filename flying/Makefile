FILE       = flying.cpp 
EXECUTABLE = flying
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
CC         = g++ -Wall -std=c++11
OBJECTS = flying.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS)

$.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
