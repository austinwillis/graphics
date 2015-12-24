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

		enum mTokenType {IDENT, LIGHT, ROTATE, TRANSLATE, 
		SCALE, PUSH, POP, IDENTITYAFFINE, 
		GLOBALAMBIENT, BACKGROUND, MINREFLECTIVITY,
		MINTRANSPARENCY, MAXRECURSIONDEPTH, CUBE, SPHERE, TORUS, PLANE,
		SQUARE, TRIANGLE, CYLINDER, CONE, TAPEREDCYLINDER,TETRAHEDRON, OCTAHEDRON, 
		DODECAHEDRON, ICOSAHEDRON,BUCKYBALL, WINEGLASS, TEAPOT,
		DIAMOND,UNION,INTERSECTION, DIFFERENCEa, MAKEPIXMAP,
		MESH, DEFAULTMATERIALS, AMBIENT, DIFFUSE,SPECULAR,
		SPECULARFRACTION, SURFACEROUGHNESS,EMISSIVE, SPECULAREXPONENT,
		SPEEDOFLIGHT, TRANSPARENCY,REFLECTIVITY, PARAMETERS, TEXTURE,
		LFTCURLY, RGHTCURLY, DEF, USE, T_NULL, F_EOF, UNKNOWN };

class Scene{
 public:
  Light *light;
  GeomObj * obj;
  Color3 background, ambient;
  int maxRecursionDepth;
  float minReflectivity, minTransparency;
 Scene():light(NULL),obj(NULL),tail(NULL)
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
  int line;
  int nextline;  
  ifstream  *file_in;
  strstream *f_in;
  strstream temp_fin;
  DefUnitStack *def_stack;
  GeomObj * tail;
  AffineStack affStk;
  Material currMtrl;
  string nexttoken(void);
  float getFloat();
  bool isidentifier(string keyword);
  void cleanUp();
  mTokenType whichtoken(string keyword); 
};
#endif


