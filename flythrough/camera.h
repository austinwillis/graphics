#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sdl.h"

class Camera{
  private:
	Point3 eye;
	Vector3 u,v,n;
	double viewAngle, aspect, nearDist, farDist; // view volume shape

  public:
	Camera(); // default constructor
	void set(Point3 eye, Point3 look, Vector3 up); // like gluLookAt()
	void set(float eyex, float eyey, float eyez, 
		 float lookx, float looky, float lookz,
		 float upx, float upy, float upz);
	void setModelViewMatrix(); // tell OpenGL where the camera is
	Point3 getEye() { return eye; }
	void roll(float angle); // roll it
	void pitch(float angle); // increase pitch
	void yaw(float angle); // yaw it
	void slide(float delU, float delV, float delN); // slide it
	void setShape(float vAng, float asp, float nearD, float farD)
	  { 
	    viewAngle = vAng;
	    aspect = asp;
	    nearDist = nearD;
	    farDist = farD;
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
	    gluPerspective(vAng,asp,nearD,farD); 
	  }
};

#endif
