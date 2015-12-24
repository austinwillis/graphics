FILE       = 
EXECUTABLE = vaulted
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lX11 -lXmu -lglut -lGL -lGLU -lm
CC         = g++ -Wall -std=c++11
OBJECTS = GlutWin.o Camera.o main.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS)

$.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
