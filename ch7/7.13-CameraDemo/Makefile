FILE       = main.cpp 
EXECUTABLE = main
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lglut -lGLU -lGL
LDLFLAGS	 = -nostartfiles -g
CC         = g++ -Wall -std=c++11
OBJECTS		 = camera.o RGBpixmap.o SDL_arch.o 

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(XLIBS) $(GLLIBS) $(LDLFLAGS)

$.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(XLIBS) $(GLLIBS)
