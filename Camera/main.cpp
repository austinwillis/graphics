#include <GL/glut.h>
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "camera.h"

using namespace std;
bool rotate = false;
float lasty, lastx;
CCamera cam;
//<<<<<<<<<<<<<<<<<<< axis >>>>>>>>>>>>>>
void axis(double length)
{ // draw a z-axis, with cone at end
	glPushMatrix();
	glBegin(GL_LINES);
	   glVertex3d(0, 0, 0); glVertex3d(0,0,length); // along the z-axis
	glEnd();
	glTranslated(0, 0,length -0.2); 
	glutWireCone(0.04, 0.2, 12, 9);
	glPopMatrix();
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<< displayWire >>>>>>>>>>>>>>>>>>>>>>
void displayWire(void)
{
glMatrixMode(GL_PROJECTION); // set the view volume shape
glLoadIdentity();
glOrtho(-4,4, -4, 4,0, 100);
glMatrixMode(GL_MODELVIEW); // position and aim the camera
glLoadIdentity();
cam.Render();
	
glClear(GL_COLOR_BUFFER_BIT); // clear the screen
glColor3d(0,0,0); // draw black lines
axis(0.5);					 // z-axis
glPushMatrix(); 
glRotated(90, 0,1.0, 0);
axis(0.5);					// y-axis
glRotated(-90.0, 1, 0, 0);
axis(0.5);					// z-axis
glPopMatrix();

glPushMatrix();
glTranslated(0.5, 0.5, 0.5); // big cube at (0.5, 0.5, 0.5)
glutWireCube(1.0);
glPopMatrix();

glPushMatrix();	
glTranslated(1.0,1.0,0);	// sphere at (1,1,0)
glutWireSphere(0.25, 10, 8);
glPopMatrix();

glPushMatrix();
glTranslated(1.0,0,1.0);	// cone at (1,0,1)
glutWireCone(0.2, 0.5, 10, 8);
glPopMatrix();
	
glPushMatrix();
glTranslated(1,1,1);
glutWireTeapot(0.2); // teapot at (1,1,1)
glPopMatrix();

glPushMatrix();
glTranslated(0, 1.0 ,0); // torus at (0,1,0)
glRotated(90.0, 1,0,0);
glutWireTorus(0.1, 0.3, 10,10);
glPopMatrix();

glPushMatrix();
glTranslated(1.0, 0 ,0); // dodecahedron at (1,0,0)
glScaled(0.15, 0.15, 0.15);
glutWireDodecahedron();
glPopMatrix();

glPushMatrix();
glTranslated(0, 1.0 ,1.0); // small cube at (0,1,1)
glutWireCube(0.25);
glPopMatrix();

glPushMatrix();
glTranslated(0, 0 ,1.0); // cylinder at (0,0,1)
GLUquadricObj * qobj;
qobj = gluNewQuadric();
gluQuadricDrawStyle(qobj,GLU_LINE);
gluCylinder(qobj, 0.2, 0.2, 0.4, 8,8);
glPopMatrix();
glFlush();
}

void moveMouse(int x, int y) {
	if (y != lasty) {
		if (y < lasty)
			cam.RotateX(-.5);
		else cam.RotateX(.5);
		lasty = y;
	}
	if (x != lastx) {
		if (x < lastx)
			cam.RotateY(-.5);
		else cam.RotateY(.5);
		lastx = x;
	}
	
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
  switch (button) {
     case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN && rotate == true) {
					cam.RotateZ(-5.0);
					break;
        }
        break;
     case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN && rotate == true) {
					cam.RotateZ(5.0);
					break;
				}
        break;
     default:
        break;
  }
  glutPostRedisplay(); // draw it again
}

void myKeyboard(unsigned char key, int x, int y) {
  switch(key) {
		case 27:
			exit(0);
		case 'w':
			cam.MoveForward(-0.1);
			break;
		case 's':
			cam.MoveForward(0.1);
			break;
		case 'a':
			cam.StrafeRight(-0.1);
			break;
		case 'd':
			cam.StrafeRight(0.1);
			break;
		case 'z':
			cam.RotateX(5.0);
			break;
		case 'c':
			cam.RotateX(-5.0);
			break;
		case 'r':
			rotate = true;
			break;
	}
  glutPostRedisplay(); // draw it again
}

void keyUp(unsigned char key, int x, int y) {
	switch (key) {
		case 'r':
			rotate = false;
			break;
		default:
			break;
	}
}
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char **argv)
{
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
glutInitWindowSize(640,640);
glutInitWindowPosition(100, 100);
glutCreateWindow("Transformation testbed - wireframes");
glutDisplayFunc(displayWire);
glutMouseFunc(myMouse);                  // register myMouse function
glutPassiveMotionFunc(moveMouse);
glutKeyboardFunc(myKeyboard);
glutKeyboardUpFunc(keyUp);
glClearColor(1.0f, 1.0f, 1.0f,0.0f);  // background is white
glViewport(0, 0, 640, 480);
glutMainLoop();
}
