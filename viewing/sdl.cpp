/* File:sdl.cpp
   scene class definition
   ----------------------------------------------------------------
   Class: CS 355			 Instructor: Dr. Morse
   Assignment: Case Study 5.5 		 Date assigned: October 28, 2008
   Programmer: David Houngninou		 Date completed: November 18, 2008*/

#include "sdl.h"
using namespace std;

// Vector3 methods
Vector3 Vector3 :: cross(Vector3 b) //return this cross b
{
  Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
  return c;
}
float Vector3 :: dot(Vector3 b) // return this dotted with b
{return x * b.x + y * b.y + z * b.z;}

void Vector3 :: normalize()//adjust this vector to unit length
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

// Color3 methods
void Color3 ::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
  red   += src.red   * refl.red;
  green += src.green * refl.green; 
  blue  += src.blue  * refl.blue;
}
void Color3:: add(Color3& colr)
{ // add colr to this color
  red += colr.red ; green += colr.green; blue += colr.blue;}

void Color3 :: build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
  v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}


//Affine4 methods
Affine4::Affine4(){ // make identity transform
  m[0] = m[5]  = m[10] = m[15] = 1.0;
  m[1] = m[2]  = m[3]  = m[4]  = 0.0;
  m[6] = m[7]  = m[8]  = m[9]  = 0.0;
  m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 :: setIdentityMatrix(){ // make identity transform
  m[0] = m[5]  = m[10] = m[15] = 1.0;
  m[1] = m[2]  = m[3]  = m[4]  = 0.0;
  m[6] = m[7]  = m[8]  = m[9]  = 0.0;
  m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 ::set(Affine4 a)// set this matrix to a
{
  for(int i = 0; i < 16; i++)
    m[i]=a.m[i];
}

//<<<<<<<<<<<<<< preMult >>>>>>>>>>>>
void Affine4 ::preMult(Affine4 n)
{// postmultiplies this with n
  float sum;
  Affine4 tmp; 
  tmp.set(*this); // tmp copy
  // following mult's : this = tmp * n
  for(int c = 0; c < 4; c++)
    for(int r = 0; r <4 ; r++)
      {
	sum = 0;
	for(int k = 0; k < 4; k++)
	  sum += n.m[4 * k + r]* tmp.m[4 * c + k];
	m[4 * c + r] = sum;
      }// end of for loops
}// end of preMult()	

//<<<<<<<<<<<< postMult >>>>>>>>>>>
void Affine4 ::postMult(Affine4 n){// postmultiplies this with n
  float sum;
  Affine4 tmp; 
  tmp.set(*this); // tmp copy
  for(int c = 0; c < 4; c++)// form this = tmp * n
    for(int r = 0; r <4 ; r++)
      {
	sum = 0;
	for(int k = 0; k < 4; k++)
	  sum += tmp.m[4 * k + r]* n.m[4 * c + k];
	m[4 * c + r] = sum;
      }// end of for loops
}

// AffineStack methods
void AffineStack :: dup()
{
  AffineNode* tmp = new AffineNode;			
  tmp->affn = new Affine4(*(tos->affn));
  tmp->invAffn = new Affine4(*(tos->invAffn));
  tmp->next = tos;
  tos = tmp;
}

void AffineStack :: setIdentity() // make top item the identity matrix
{ 
  assert(tos != NULL);
  tos->affn->setIdentityMatrix();
  tos->invAffn->setIdentityMatrix();
}

void AffineStack :: popAndDrop()
{
  if(tos == NULL) return; // do nothing
  AffineNode *tmp = tos;
  tos = tos->next;
  delete tmp; // should call destructor, which deletes trices
}
	
void AffineStack :: releaseAffines()
{ // pop and drop all remaining items 
  while(tos) popAndDrop();
}

void AffineStack :: rotate(float angle, Vector3 u)
{
  Affine4 rm; // make identity matrix
  Affine4 invRm;
  u.normalize(); // make the rotation axis unit length
  float ang = angle * 3.14159265/ 180; // deg to  
  float c = cos(ang), s = sin(ang);
  float mc = 1.0 - c;
  //fill the 3x3 upper left matrix - Chap.5 p. 29
  rm.m[0] = c + mc * u.x * u.x;
  rm.m[1] = mc * u.x * u.y + s * u.z;
  rm.m[2] = mc * u.x * u.z - s * u.y;
  rm.m[4] = mc * u.y * u.x - s * u.z;
  rm.m[5] = c + mc * u.y * u.y;
  rm.m[6] = mc * u.y * u.z + s * u.x;
  rm.m[8] = mc * u.z * u.x + s * u.y;
  rm.m[9] = mc * u.z * u.y - s * u.x;
  rm.m[10] = c + mc * u.z * u.z;
  // same for inverse : just sign of s is changed
  invRm.m[0] = c + mc * u.x * u.x;
  invRm.m[1] = mc * u.x * u.y - s * u.z;
  invRm.m[2] = mc * u.x * u.z + s * u.y;
  invRm.m[4] = mc * u.y * u.x + s * u.z;
  invRm.m[5] = c + mc * u.y * u.y;
  invRm.m[6] = mc * u.y * u.z - s * u.x;
  invRm.m[8] = mc * u.z * u.x - s * u.y;
  invRm.m[9] = mc * u.z * u.y + s * u.x;
  invRm.m[10] = c + mc * u.z * u.z;
  tos->affn->postMult(rm);
  tos->invAffn->preMult(invRm);
}

void AffineStack :: scale(float sx, float sy, float sz)
{ // post-multiply top item by scaling
#define sEps 0.00001
  Affine4 scl;// make an identity
  Affine4 invScl;
  scl.m[0]  = sx; 
  scl.m[5]  = sy; 
  scl.m[10] = sz;// adjust it to a scaling matrix
  if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < sEps)
    {
      cerr << "degenerate scaling transformation!\n";
    }
  invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; invScl.m[10] = 1/sz;
  tos->affn->postMult(scl); //
  tos->invAffn->preMult(invScl);
}	

void AffineStack :: translate(Vector3 d)
{
  Affine4 tr; // make identity matrix
  Affine4 invTr;
  tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
  invTr.m[12] = -d.x;	invTr.m[13] = -d.y; invTr.m[14] = -d.z;
  tos->affn->postMult(tr);
  tos->invAffn->preMult(invTr);
}

// Material methods
void Material :: setDefault(){
  textureType = 0; // for none
  numParams = 0;
  reflectivity = transparency = 0.0;
  speedOfLight = specularExponent = 1.0;
  specularFraction = 0.0;
  surfaceRoughness = 1.0;
  ambient.set(0.1f,0.1f,0.1f);
  diffuse.set(0.8f,0.8f,0.8f);
  specular.set(0,0,0);
  emissive.set(0,0,0);
}

void Material :: set(Material& m)
{
  textureType = m.textureType;
  numParams = m.numParams;
  for(int i = 0; i < numParams; i++) params[i] = m.params[i];
  transparency = m.transparency;
  speedOfLight = m.speedOfLight;
  reflectivity = m.reflectivity;
  specularExponent = m.specularExponent;
  specularFraction = m.specularFraction;
  surfaceRoughness = m.surfaceRoughness;
  ambient.set(m.ambient);
  diffuse.set(m.diffuse);
  specular.set(m.specular);
  emissive.set(m.emissive);
}

// Shape methods
void Shape :: tellMaterialsGL()
{
  float amb[4],diff[4],spec[4], emiss[4];
  float zero[] = {0,0,0,1};
  mtrl.ambient.build4tuple(amb); // fill the array
  mtrl.diffuse.build4tuple(diff);
  mtrl.specular.build4tuple(spec);
  mtrl.emissive.build4tuple(emiss);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
  glMaterialf(GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
}

// DefUnitStack methods
void DefUnitStack :: push(string n, string s) { 
  D4S *temp_d4s = new D4S;
  temp_d4s->current = new DefUnit(n, s);
  temp_d4s->next = stack;
  stack = temp_d4s;
}

void DefUnitStack :: print() {
  D4S *temp = stack;
  string t;
  while (temp) {
    cout << temp->current->name << ":" ;
    cout << temp->current->stuff << endl;
    temp = temp->next;
  }
}

int DefUnitStack :: search(string s) {
  D4S *temp = stack;
  while (temp) {
    if ( temp->current->name == s ) {
      return(1);
    }
    temp = temp->next;
  }
  return(0);
}

string DefUnitStack :: contents(string s) {
  D4S *temp = stack;
  while (temp) {
    if (temp->current->name == s ) {
      return(temp->current->stuff);
    }
    temp = temp->next;
  }
  return(NULL);
}

void DefUnitStack :: release()
{
  while(stack)
    {
      D4S* tmp = stack; // grab it
      //cerr << "releasing def_stack item: "<< tmp->current->name<< endl;
      stack = stack->next; // advance p
      delete tmp->current; // release 2 strings
      delete tmp; // release node
	}
  stack = NULL;
}

// Mesh methods
Mesh :: Mesh(){
  numVerts = numFaces = numNorms = 0; 
  pt = NULL; norm  =  NULL; face = NULL;
  lastVertUsed = lastNormUsed = lastFaceUsed = -1;
}

void Mesh :: freeMesh()
{ // free up memory used by this mesh.
  delete [] pt; // release whole vertex list
  delete [] norm;
  for(int f = 0; f < numFaces; f++)
    delete[] face[f].vert; // delete the vert[] array of this face
  delete [] face;
}

int Mesh :: isEmpty() 
{
  return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);
}

void Mesh :: makeEmpty() 
{ 
  numVerts = numFaces = numNorms = 0;
}

void Mesh :: drawOpenGL()
{ 
  tellMaterialsGL(); 	glPushMatrix();
  glMultMatrixf(transf.m); 
  drawMesh();	
  //if(doEdges) drawEdges();
  glPopMatrix();
} 
Mesh :: Mesh(string fname){ // read this file to build mesh
  numVerts = numFaces = numNorms = 0; 
  pt = NULL; norm  =  NULL; face = NULL;
  lastVertUsed = lastNormUsed = lastFaceUsed = -1;
  readMesh(fname);
}
Vector3 Mesh :: newell4(int indx[])
{ /* return the normalized normal to face with vertices
     pt[indx[0]],...,pt[indx[3]]. i.e. indx[] contains the four indices 
     into the vertex list to be used in the Newell calculation */
  Vector3 m;
  for(int i = 0; i < 4 ; i++)
    {
      int next = (i== 3) ? 0 : i + 1; // which index is next?
      int f = indx[i], n = indx[next]; // names for the indices in the pair
      m.x += (pt[f].y - pt[n].y) * (pt[f].z + pt[n].z);
      m.y += (pt[f].z - pt[n].z) * (pt[f].x + pt[n].x);
      m.z += (pt[f].x - pt[n].x) * (pt[f].y + pt[n].y);
    }
  m.normalize();
  return m;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: readMesh(string fname)
{
  cout  << fname << endl;
  fstream inStream;
  inStream.open(fname.c_str(), ios ::in); //open needs a c-like string
  if(inStream.fail() || inStream.eof()) 
    {
      cout << "can't open file or eof: " << fname << endl; 
      makeEmpty();return;
    }
  
  inStream >> numVerts;
  pt = new Point3[numVerts];        assert(pt != NULL);
  for(int i = 0; i < numVerts; i++) 	// read in the vertices
    inStream >> pt[i].x >> pt[i].y >> pt[i].z;
  
  inStream >> numNorms;
  norm = new Vector3[numNorms];     assert(norm != NULL);
  for(int ii = 0; ii < numNorms; ii++) 	// read in the normals
    inStream >> norm[ii].x >> norm[ii].y >> norm[ii].z;
  
  inStream >> numFaces;
  face = new Face[numFaces];        assert(face != NULL);
  
  for(int f = 0; f < numFaces; f++)   // read in face data
    {
      inStream >> face[f].nVerts;
      int n = face[f].nVerts;
      face[f].vert = new VertexID[n]; assert(face[f].vert != NULL);
      for(int k = 0; k < n; k++) 		// read vertex indices for this face
	inStream >> face[f].vert[k].vertIndex;
      for(int kk = 0; kk < n; kk++) 		// read normal indices for this face
	inStream >> face[f].vert[kk].normIndex;
    }
  inStream.close();
} // end of readMesh

//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>
void Mesh :: drawMesh()
{ // draw each face of this mesh using OpenGL: draw each polygon.
  if(isEmpty()) return; // mesh is empty
  for(int f = 0; f < numFaces; f++)
    {
      int n = face[f].nVerts;
      glBegin(GL_POLYGON);
      for(int v = 0; v < n; v++)
	{
	  int in = face[f].vert[v].normIndex;
	  assert(in >= 0 && in < numNorms);
	  glNormal3f(norm[in].x, norm[in].y, norm[in].z);
	  int iv = face[f].vert[v].vertIndex; assert(iv >= 0 && iv < numVerts);
	  glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
	}
      glEnd();	
	}
  glFlush();
}		

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< write mesh>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: writeMesh(char * fname)
{ // write this mesh object into a new Chapter 6 format file.
  if(numVerts == 0 || numNorms   == 0 || numFaces == 0) return; //empty
  fstream outStream(fname, ios ::out); // open the output stream
  if(outStream.fail()) {cout << "can't make new file: " << fname << endl; 
	return;}
  outStream << numVerts << " " << numNorms << " " << numFaces << "\n";
  // write the vertex and vertex normal list
  for(int i = 0; i < numVerts; i++)
    outStream << pt[i].x   << " " << pt[i].y   << " " << pt[i].z << "\n";
  for(int ii = 0; ii < numNorms; ii++)
    outStream  << norm[ii].x << " " << norm[ii].y << " " << norm[ii].z << "\n";
  // write the face data
  for(int f = 0; f < numFaces; f++)
    {
      int n = face[f].nVerts;
      outStream << n << "\n";
      for(int v = 0; v < n; v++)// write vertex indices for this face
	outStream << face[f].vert[v].vertIndex << " ";	outStream << "\n";
      for(int k = 0; k < n; k++)	// write normal indices for this face 
	outStream << face[f].vert[k].normIndex << " "; outStream << "\n";
    }
  outStream.close();
}

