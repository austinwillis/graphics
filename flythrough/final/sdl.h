#ifndef _BasicStuff
#define _BasicStuff
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>

using namespace std;


#include <assert.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

 
class Point3{ 
 public: 
  float x,y,z;
  void set(float dx, float dy, float dz){x = dx; y = dy; z = dz;}
  void set(Point3& p){x = p.x; y = p.y; z = p.z;} 	
  Point3(float xx,     float yy, float zz){x = xx; y = yy; z = zz;}
  Point3(){x = y = z = 0;}
  void build4tuple(float v[])
  {
    v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
  }	
};

class Vector3{ 
public: 
	float x,y,z;

	void set(float dx, float dy, float dz)
	  { x = dx; y = dy; z = dz;} 
	void set(const Vector3& v)
	  { x = v.x; y = v.y; z = v.z;}
	void flip()
	  {x = -x; y = -y; z = -z;} // reverse this vector
	void setDiff(Point3& a, Point3& b)//set to difference a - b
	  { x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}
	void normalize()//adjust this vector to unit length
	{		
		double sizeSq = x * x + y * y + z * z;
		if(sizeSq < 0.0000001)
		{
			cerr << "\nnormalize() sees vector (0,0,0)!";
			return; // does nothing to zero vectors;
		}
		float scaleFactor = 1.0/(float)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
	}
	Vector3(float xx, float yy, float zz)
	  {x = xx; y = yy; z = zz;}
	Vector3(const Vector3& v)
	  {x = v.x; y = v.y; z = v.z;}
	Vector3()
	  {x = y = z = 0;} //default constructor
	Vector3 cross(Vector3 b) //return this cross b
	{
	   Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	   return c;
	}
	float dot(Vector3 b) // return this dotted with b
	  {return x * b.x + y * b.y + z * b.z;}
	Vector3 operator*(float scalar) // multiply vector by scalar
	  {
	    Vector3 c(scalar*x, scalar*y, scalar*z);
	    return c;
	  }
	Vector3 friend operator*(float scalar,Vector3 v) // multiply vector by scalar
	  {
	    Vector3 c(scalar*v.x, scalar*v.y, scalar*v.z);
	    return c;
	  }
	Vector3 operator+(const Vector3& v)
	  {
	    Vector3 c(x+v.x, y+v.y, z+v.z);
	    return c;
	  }
	Vector3 operator-(const Vector3& v)
	  {
	    Vector3 c(x-v.x, y-v.y, z-v.z);
	    return c;
	  }
};


class Color3 {
 public:
  float red, green, blue;
  Color3(){red = green = blue = 0;}
  Color3(float r, float g, float b){red = r; green = g; blue = b;}
  Color3(Color3& c){red = c.red; green = c.green; blue = c.blue;}
  void set(float r, float g, float b){red = r; green = g; blue = b;}
  void set(Color3& c) 
  {red = c.red; green = c.green; blue = c.blue;}
  void add(float r, float g, float b)
  {red += r; green += g; blue += b;}
  void add(Color3& src, Color3& refl);
  void add(Color3& colr);
  void build4tuple(float v[]);
};


class Light{
 public:
  Point3 pos;
  Color3 color;
  Light* next;
  void setPosition(Point3 p){pos.set(p);}
  void setColor(Color3 c){color.set(c);}
  Light(){next = NULL;}
};

class Affine4 {
 public:
  float m[16];
  Affine4();
  void setIdentityMatrix();
  void set(Affine4 a);
  void preMult(Affine4 n);
  void postMult(Affine4 n);
};


class AffineNode{
 public:
  Affine4 * affn;
  Affine4 * invAffn;
  AffineNode * next;
  AffineNode()
    {
      next = NULL;
      affn = new Affine4; // new affine with identity in it
      invAffn = new Affine4; // and for the inverse
    }
  ~AffineNode() //destructor
    {
      delete affn;
      delete invAffn;
    }
};

class AffineStack{

 public:
  AffineNode * tos;
  AffineStack()
    {
      tos = new AffineNode; // node with identity in it
      tos->next = NULL;
    }
  void dup();
  void setIdentity();// make top item the identity matrix
  void popAndDrop();
  void releaseAffines(); // pop and drop all remaining items 
  void rotate(float angle, Vector3 u);
  void scale(float sx, float sy, float sz);
  void translate(Vector3 d);
}; // end of AffineStack class

class Material{
 public:
  Color3 ambient, diffuse, specular, emissive;
  int numParams; // for textures
  float params[10]; // for textures
  int textureType;  // 0 for none, neg for solids, pos for images
  float specularExponent, reflectivity, transparency, speedOfLight;
  float specularFraction, surfaceRoughness;	
  void setDefault();
  void set(Material& m);
}; // end of Material

class GeomObj{
 public: 
  //IntRect scrnExtnt;
  GeomObj * next;
 GeomObj(): next(NULL){}
  virtual void loadStuff(){}
  virtual void drawOpenGL(){}
  virtual void tellMaterialsGL(){}
};
 
class Boolean: public GeomObj{
 public:
  GeomObj *left, *right;
 Boolean():left(NULL),right(NULL){}
  virtual void drawOpenGL()
	{ // just draw its children
	  if(left)left->drawOpenGL();
	  if(right)right->drawOpenGL();
	}
};

class UnionBool : public Boolean{
 public:
  UnionBool(){Boolean();} //constructor
};

class IntersectionBool : public Boolean{
 public:
  IntersectionBool(){Boolean();}
};

class DifferenceBool : public Boolean{
public:
	DifferenceBool(){Boolean();} 
};

class Shape: public GeomObj{
 public:
  Material mtrl;
  Affine4 transf,invTransf;
  
  Shape(){mtrl.textureType = 0; mtrl.numParams = 0;}
  void setMaterial(Material& mt){mtrl.set(mt);}
  void tellMaterialsGL();
  virtual void drawOpenGL(){}
};

class Cube : public Shape{
 public:	
  Cube(){}
  void drawOpenGL()
  {
    tellMaterialsGL(); glPushMatrix();
    glMultMatrixf(transf.m); //load affine
    glEnable(GL_NORMALIZE);
    glutSolidCube(2.0);	// a cuber with vertices -1 to +1
    glPopMatrix();
  }
};

class Cone : public Shape{
public:
  Cone(){}
  void drawOpenGL()
  {
    tellMaterialsGL(); glPushMatrix();
    glMultMatrixf(transf.m); //load affine
    glEnable(GL_NORMALIZE);
    glutSolidCone(1.0, 1.0 , 20, 20);	// a cone
    glPopMatrix();
  } 
};

class Sphere : public Shape{
 public:
  void drawOpenGL()	
  { 
    tellMaterialsGL(); 	glPushMatrix();	
    glMultMatrixf(transf.m); 
    glutSolidSphere(1.0,20,20);	
    glPopMatrix();
  }
  Sphere() { }
};
 
class TaperedCylinder : public Shape{
 public:
  float smallRadius;
  TaperedCylinder(){}
  void drawOpenGL(){tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    GLUquadricObj * qobj = gluNewQuadric();
    gluCylinder(qobj,1.0,smallRadius,1.0, 30,3);
    gluDisk(qobj, 0.0, 1.0, 10,4); //base at z = 0
    glTranslatef(0,0,1.0);
    gluDisk(qobj, 0.0, smallRadius, 30,2); //cap at z = 1
    glPopMatrix();
  }
};

class Square : public Shape{
 public:
  Square(){}
  void drawOpenGL()
  {
    tellMaterialsGL(); 	
    glPushMatrix();
    glMultMatrixf(transf.m); 
    glEnable(GL_NORMALIZE);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1.0);
    glVertex3f(-1.0,-1.0,0);
    glVertex3f( 1.0,-1.0,0);
    glVertex3f( 1.0, 1.0,0);
    glVertex3f(-1.0, 1.0,0);
    glEnd();
    glPopMatrix();
  }
};

class Triangle : public Shape{
 public:
  Triangle(){}
  void drawOpenGL()
  {
    tellMaterialsGL(); 	
    glPushMatrix();
    glMultMatrixf(transf.m); 
    glEnable(GL_NORMALIZE);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1.0);
    glVertex3f(-1.0,-1.0,0);
    glVertex3f( 1.0,-1.0,0);
    glVertex3f( 1.0, 1.0,0);
    glEnd();
    glPopMatrix();
  }
};

class Plane : public Shape{
 public:
  Plane() {}
  void drawOpenGL()
  {
    tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    glEnable(GL_NORMALIZE);
    glBegin(GL_QUADS);
    glNormal3f(0,0,1.0);
    glVertex3f(-10.0,-10.0,0);
    glVertex3f( 10.0,-10.0,0);
    glVertex3f( 10.0, 10.0,0);
    glVertex3f(-10.0, 10.0,0);
    glEnd();
    glPopMatrix();
  }
};

class VertexID{public: int vertIndex, normIndex;};

class Face{
 public:
  int nVerts;
  VertexID * vert; // array of vertex and normal indices
  Face(){ nVerts = 0; vert = NULL;}
  ~Face(){delete[] vert; nVerts = 0;}
};

class Mesh : public Shape{
 private:
  int numVerts, numNorms, numFaces;
  Point3 *pt; // array of points
  Vector3 *norm; // array of normals
  Face *face; // array of faces
  int lastVertUsed;
  int lastNormUsed;
  int lastFaceUsed;
 public:
  void readMesh(string fname);
  void writeMesh(char* fname);
  void printMesh();
  void drawMesh();
  void drawEdges();
  void freeMesh();
  int isEmpty(); 	
  void makeEmpty(); 
  Mesh();
  virtual void drawOpenGL();	
  Mesh(string fname);
  Vector3 newell4(int indx[]);
  string meshFileName; // holds file name for this Mesh
}; // end of Mesh class

class Torus : public Shape{
 public:
  void drawOpenGL(){ 
    tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    glutSolidTorus(0.2,1.0,20,20);	
    //if(doEdges) glutWireTorus(0.2,1.0,10,12);	
    glPopMatrix();}
};
 
class Teapot : public Shape{
 public:
  void drawOpenGL(){ tellMaterialsGL(); glPushMatrix();
    glMultMatrixf(transf.m); 
    glutSolidTeapot(1.0); glPopMatrix();}
};


class Tetrahedron : public Shape{
 public:
  void drawOpenGL(){ tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); glutSolidTetrahedron();	glPopMatrix();
  }
};

class Octahedron : public Shape{
 public:
  void drawOpenGL(){ tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    glutSolidOctahedron();	glPopMatrix();}
  
};

class Dodecahedron : public Shape{
 public:
  void drawOpenGL(){tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    glutSolidDodecahedron();	glPopMatrix();} 
};

class Icosahedron : public Shape{
 public:
  void drawOpenGL(){ tellMaterialsGL(); 	glPushMatrix();
    glMultMatrixf(transf.m); 
    glutSolidIcosahedron();	glPopMatrix();} 
};

class DefUnit {
 public:
  string    name, stuff;
  DefUnit(string n, string s) {stuff = s;name = n;}
};
class DefUnitStack {
 public:
  DefUnitStack() {stack = NULL;}
  void push(string n, string s);
  void print();
  int search(string s);
  string contents(string s);
  void release();
 private:
  struct D4S {
    DefUnit *current;
    struct D4S *next;
  } d4s;
  D4S *stack;
};

#endif



