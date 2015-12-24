FILE       = main.cpp 
EXECUTABLE = main
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lglut -lGLU -lGL -std=c++11
CC         = g++
OBJECTS		 = main.o Camera.o GlutWin.cpp 

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS)

$.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
