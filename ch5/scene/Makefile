#
# Makefile for csserver to create executables using OpenGL.
#    Glut, and X-Windows
#
# CS 355 Computer Graphics
# Fall 2015 
#
# University of Evansville
#
# $Id$
#
##### MACROS #####

FILE       = scene.cpp 
EXECUTABLE = scene	 
XLIBS      = -L/usr/X11R6/lib -lglut -lGLU -lGL 
GLLIBS     = -lglut -lGLU -lGL
CC         = clang++

##### COMPILATION ####

default: $(FILE)
	$(CC) $(FILE) $(XLIBS) $(GLLIBS)  -o $(EXECUTABLE) 

