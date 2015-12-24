#include <math.h>
#include "camera.h"

Camera::Camera()
{viewAngle=aspect=nearDist=farDist=0;} // default constructor

void Camera::set(Point3 Eye, Point3 look, Vector3 up)
{
  eye.set(Eye); // store the given eye position
  n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z); // make n
  u.set(up.cross(n)); // make u = up X n
  n.normalize(); u.normalize(); // make them unit length
  v.set(n.cross(u));  // make v =  n X u
  setModelViewMatrix(); // tell OpenGL 
}

void Camera::set(float eyex, float eyey, float eyez, 
		 float lookx, float looky, float lookz,
		 float upx, float upy, float upz)
{	// create a modelview matrix and send it to OpenGL
  Point3 Eye(eyex,eyey,eyez);
  Point3 look(lookx,looky,lookz);
  Vector3 up(upx,upy,upz);

  eye.set(Eye); // store the given eye position
  n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z); // make n
  u.set(up.cross(n)); // make u = up X n
  n.normalize(); u.normalize(); // make them unit length
  v.set(n.cross(u));  // make v =  n X u
  setModelViewMatrix(); // tell OpenGL 
}

void Camera::setModelViewMatrix(void)
{ // load modelview matrix with existing camera values
  float m[16];
  Vector3 eVec(eye.x, eye.y, eye.z); // a vector version of eye 
  m[0] =  u.x; m[4] =  u.y; m[8]  =  u.z;  m[12] = -eVec.dot(u);
  m[1] =  v.x; m[5] =  v.y; m[9]  =  v.z;  m[13] = -eVec.dot(v);
  m[2] =  n.x; m[6] =  n.y; m[10] =  n.z;  m[14] = -eVec.dot(n);
  m[3] =  0;   m[7] =  0;   m[11] =  0;    m[15] = 1.0;
  glMatrixMode(GL_MODELVIEW);
  //glPushMatrix();
  glLoadMatrixf(m); // load OpenGL's modelview matrix
  //glPopMatrix();
}

void Camera::slide(float delU, float delV, float delN)
{
  eye.x += delU * u.x + delV * v.x + delN * n.x;
  eye.y += delU * u.y + delV * v.y + delN * n.y;
  eye.z += delU * u.z + delV * v.z + delN * n.z;
  setModelViewMatrix();		// inform OpenGL
}

void Camera::roll(float angle)
{
  float cs = cos(3.14159265/180 * angle);
  float sn = sin(3.14159265/180 * angle);
  Vector3 t = u; 		// remember old u

  u.set(cs*t + sn*v);
  v.set(cs*v - sn*u);

  setModelViewMatrix();		// inform OpenGL
}

void Camera::pitch(float angle) // increase pitch
{
  float cs = cos(3.14159265/180 * angle);
  float sn = sin(3.14159265/180 * angle);
  Vector3 n1 = cs * n + sn * v;
  Vector3 v1 = cs * v - sn * n;

  n.set(n1);
  v.set(v1);

  setModelViewMatrix();		// inform OpenGL
}

void Camera::yaw(float angle) // increase pitch
{
  float cs = cos(3.14159265/180 * angle);
  float sn = sin(3.14159265/180 * angle);
  Vector3 n1 = cs*n + sn*u; 
  Vector3 u1 = (cs)*u - sn*n;

  u.set(u1);
  n.set(n1);
  setModelViewMatrix();		// inform OpenGL
}
