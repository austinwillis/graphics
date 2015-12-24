FILE       = flying.cpp 
EXECUTABLE = flying
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
CC         = g++ -Wall -std=c++11

default: $(FILE)
	$(CC) $(FILE) $(XLIBS) $(GLLIBS)  -o $(EXECUTABLE) 
