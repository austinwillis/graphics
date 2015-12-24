/*  wireframe.cpp
 *  This program demonstrates the drawing wireframe drawing of a set
 *  of primitive shapes.
 *  Use the left and middle buttons to zoom in and out.
 *  Click the right button to exit.
*/
//#include <windows.h>  //when using Windows 95/98/NT/2000
// for CS domain
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
// for CSLab
#include <GL/glut.h>
#include <GL/gl.h>
#include <X11/Xlib.h>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;
bool rotate = false;
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

float lastx, lasty;
//<<<<<<<<<<<<<<<<<<< axis >>>>>>>>>>>>>>
void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh 
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the 
    glTranslated(-xpos,-ypos,-zpos); //translate the screen
}

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
glOrtho(-2.0*64/48.0, 2.0*64/48.0, -2.0, 2.0, 0.1, 100);
glMatrixMode(GL_MODELVIEW); // position and aim the camera
glLoadIdentity();
camera();
	
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
int diffx=x-lastx; //check the difference between the 
    int diffy=y-lasty; //check the difference between the 
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += (float) diffy; //set the xrot to xrot with the addition
    yrot += (float) diffx;    //set the xrot to yrot with the addition
glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
  switch (button) {
     case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN && rotate == true) {
					cout << "rotate left" << endl;
        }
        break;
     case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN && rotate == true) {
					cout << "rotate right" << endl;
				}
        break;
     default:
        break;
  }
  glutPostRedisplay(); // draw it again
}

void myKeyboard(unsigned char key, int x, int y) {
if (key=='q')
    {
    xrot += 1;
    if (xrot >360) xrot -= 360;
    }

    if (key=='z')
    {
    xrot -= 1;
    if (xrot < -360) xrot += 360;
    }

    if (key=='w')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos += float(sin(yrotrad)) ;
    zpos -= float(cos(yrotrad)) ;
    ypos -= float(sin(xrotrad)) ;
    }

    if (key=='s')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f);
    xpos -= float(sin(yrotrad));
    zpos += float(cos(yrotrad)) ;
    ypos += float(sin(xrotrad));
    }

    if (key=='d')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += float(cos(yrotrad)) * 0.2;
    zpos += float(sin(yrotrad)) * 0.2;
    }

    if (key=='a')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= float(cos(yrotrad)) * 0.2;
    zpos -= float(sin(yrotrad)) * 0.2;
    }

    if (key==27)
    {
    exit(0);
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
glutInitWindowSize(640,480);
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
