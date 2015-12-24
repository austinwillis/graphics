FILE   = main.cpp scene.cpp sdl.cpp camera.cpp
INCDIR = /usr/X11R6/include/GL -I/usr/X11R6/include
XLIBS  = -L/usr/X11R6/lib -lglut -lGLU -lGL -lm -lX11 -lXmu -lXt -lXext -lSM -lICE -lX11

CC = g++

default: $(FILE)
	$(CC) -I$(INCDIR) $(FILE) $(XLIBS)  -o flythrough -Wno-deprecated

