FILE       = 
EXECUTABLE = Flag
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
CC         = g++ -Wall -std=c++11
OBJECTS = main.o Flag.o textures.o vectors.o camera.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) lX11 -lXi -lXmu -lglut -lGL -lGLU -lm


$.o: %.cpp
	$(CC) -c -o $@ $< lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
