/* File: scene.h
   scene header file
   ----------------------------------------------------------------
   Class: CS 355			 Instructor: Dr. Morse
   Assignment: Case Study 5.5 		 Date assigned: October 28, 2008
   Programmer: David Houngninou		 Date completed: November 18, 2008*/

#ifndef _SCENE_H
#define _SCENE_H
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "sdl.h"

using namespace std;

//+++++++++++++ TokenType +++++++++++++
		enum mTokenType {IDENT, LIGHT, ROTATE, TRANSLATE, 
		SCALE, PUSH, POP, IDENTITYAFFINE, 
		GLOBALAMBIENT, BACKGROUND, MINREFLECTIVITY,
		MINTRANSPARENCY, MAXRECURSIONDEPTH, CUBE, SPHERE, TORUS, PLANE,
		SQUARE, CYLINDER, CONE, TAPEREDCYLINDER,TETRAHEDRON, OCTAHEDRON, 
		DODECAHEDRON, ICOSAHEDRON,BUCKYBALL, WINEGLASS, TEAPOT,
		DIAMOND,UNION,INTERSECTION, DIFFERENCEa, MAKEPIXMAP,
		MESH, DEFAULTMATERIALS, AMBIENT, DIFFUSE,SPECULAR,
		SPECULARFRACTION, SURFACEROUGHNESS,EMISSIVE, SPECULAREXPONENT,
		SPEEDOFLIGHT, TRANSPARENCY,REFLECTIVITY, PARAMETERS, TEXTURE,
		LFTCURLY, RGHTCURLY, DEF, USE, T_NULL, F_EOF, UNKNOWN };

class Scene{
 public:
  Light *light;  // attach linked list of lights here
  GeomObj * obj;  // attach the object list here
  Color3 background, ambient;
  int maxRecursionDepth;
  //must #include RGBpixmap.h to have following texture fields
  //RGBpixmap pixmap[8]; //list of attached pixmaps
  float minReflectivity, minTransparency;
  //bool isInShadow(Ray& f); // for ray tracing: implementation left to the reader
 Scene():light(NULL),obj(NULL),tail(NULL) //default constructor
    {
      currMtrl.setDefault();
      background.set(0,0,0.6f);
      ambient.set(0.1f,0.1f,0.1f);
      minReflectivity = 0.5;
      minTransparency = 0.5;
      maxRecursionDepth = 3;
    } 
  Scene(string fname){Scene(); read(fname);}
  void freeScene(); 
  void makeLightsOpenGL();
  void drawSceneOpenGL();
  bool read(string fname);
  GeomObj* getObject();
 private:
  // private stuff used only for reading a scene
  int line;
  int nextline;  
  ifstream  *file_in;
  strstream *f_in;
  strstream temp_fin;
  DefUnitStack *def_stack;
  GeomObj * tail; // tail of object list
  AffineStack affStk; // affine stack
  Material currMtrl;
  string nexttoken(void);
  float getFloat();
  bool isidentifier(string keyword);
  void cleanUp();
  mTokenType whichtoken(string keyword); 
}; // end of Scene.h

#endif


