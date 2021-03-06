#include <GL/glut.h>
#include "Vector3.h"
#include <cstdlib>
#include <iostream>
#include "mat.h"
#include "Angel.h"
#define PI_OVER_360 0.0087266462599716478846184538424431

using namespace std;

// THESE GLOBALS MAY NOT BE MODIFIED
//

// Fixed Screen size  
const GLdouble screenWidth = 640;
const GLdouble screenHeight = 480;

// Fixed constants related to the perspective 
// Use this to form matrix P  
//
const GLdouble aspect = 640.0/480.0;
const GLdouble viewAngle = 60;
const GLdouble N = 1, F=400;

// Globals to change the camera 
// The pipeline should compute the V matrix using these 
// values listed here
//
GLdouble eyex=50,eyey=100,eyez=100;  // point eye x,y, z coordinates
GLdouble upx=0, upy=0, upz=1;        // vector up x,y,x coordinates
GLdouble lookx=0, looky=0, lookz=0;  // point lookat

// Globals that is returned from pipeline
GLdouble pntx, pnty;

// Prototype for the pipeline function
//
void pipeLine(GLdouble x, GLdouble y, GLdouble z);
void BuildPerspProjMat(float *m, float fov, float aspect,float znear, float zfar);

// Basic initialization for OpenGL
//
// THIS FUNCTION MAY NOT BE CHANGED
//
void myInit(void){
    glClearColor(1,1,1,0);
    glColor3f(0.0f,0.0f,0.0f);
    glPointSize(2);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // These set up a "pass through" for the 2D points pipeline generates
    // to give to the OpenGL  pipeline. World window and screen window 
    // are the same, Projection is the identity.
    // 
    glViewport(0,0,(GLint)screenWidth,(GLint)screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,screenWidth,0,screenHeight);
}

// This is a primitive draw line function which does 2D rendering
//
// THIS FUNCTION MAY NOT BE CHANGED
//
void drawLine(GLdouble x1, GLdouble y1, GLdouble z1,
                  GLdouble x2, GLdouble y2, GLdouble z2){

    GLdouble x,y,u,v;

    // Push the 3D points through the pipeline getting the
    // the transformed 2D point which is the orthographic
    // projection onto the viewport
    // 
    pipeLine(x1,y1,z1);
    x = pntx; y = pnty;
    pipeLine(x2,y2,z2);
    u = pntx; v = pnty;

    // Display on the viewport (2D)
    //
    glBegin(GL_LINES);
        glVertex2d(x,y);
        glVertex2d(u,v);
    glEnd();
}

// This routine sets the OpenGL parameters and renders the 
// picture of a simple 3D house. This is a cube with a
// pyramid on top. The top is red (see the working version).
//
// THIS FUNCTION MAY NOT BE CHANGED
//
void myDisplay(void){

    myInit();

    drawLine(0,0,0, 100,0,0);
    drawLine(50,0,0, 50,0,50);
    drawLine(50,0,50, 0,0,50);
    drawLine(0,0,100, 0,0,0);
    
    drawLine(0,50,0, 50,50,0);
    drawLine(50,50,0, 50,50,50);
    drawLine(50,50,50, 0,50,50);
    drawLine(0,50,50, 0,50,0);

    drawLine(0,0,0, 0,100,0);
    drawLine(50,0,0, 50,50,0);
    drawLine(50,0,50, 50,50,50);
    drawLine(0,0,50, 0,50,50);

    glColor3f(1,0,0);
    drawLine(0,50,0, 25,75,25);
    drawLine(50,50,0, 25,75,25);
    drawLine(50,50,50, 25,75,25);
    drawLine(0,50,50, 25,75,25);
    
    glFlush();
}

// OpenGL pipeline. Takes a 3D point and returns 
// a 2D point in the global variables pntx and pnty
// which is the point on the view port.
//
// Assume the world window is W.l=-150, W.r=150, W.b=-150, W.t=150
//
// The 3D point is transformed via the V matrix where
// the matrix M is the identity matrix and then tranformed by the
// perspecitve transformation matrix P.
// 
// The orthographic projection (x*,y*) is then mapped from the 
// world window (see above for the coordinates) to the viewport which is 
// V.l=0, V.r=640, V.b=0, V.t=480.
//
// DO NOT WORRY ABOUT CLIPPING.
//
// Use the global variables
//    eyex, eyey, eyez, 
//    lookx, looky, lookz,
//    upx, upy, upz, 
//    viewangle, aspect, N, F 
//
//    and use the world window coordinates above
//    to compute the needed transformations
//
void pipeLine(GLdouble x, GLdouble y, GLdouble z){

    // Return your x* and y* values into the global
    // variables pntx, pnty.
	vec3 eyevec(eyex, eyey, eyez);
	vec3 lookatvec(lookx, looky, lookz);
	vec3 upvec(upx, upy, upz);

	vec3 zaxis = eyevec - lookatvec;
	zaxis = normalize(zaxis);
	vec3 xaxis = cross(upvec, zaxis);
	xaxis = normalize(xaxis);
	vec3 yaxis = cross(zaxis,xaxis);

	mat4 viewMatrix(
        vec4(       xaxis.x,            yaxis.x,            zaxis.x,      0 ),
        vec4(       xaxis.y,            yaxis.y,            zaxis.y,      0 ),
        vec4(       xaxis.z,            yaxis.z,            zaxis.z,      0 ),
        vec4( -dot( xaxis, eyevec ), -dot( yaxis, eyevec ), -dot( zaxis, eyevec ), 1 )
        );

	float range = N - F;
	float tanhalf = tanf((viewAngle * (M_PI / 180) /2));

	mat4 projMatrix(
		vec4(1/(tanhalf * aspect), 0, 0, 0),
		vec4(0, 1/tanhalf, 0, 0),
		vec4(0,0,( - N - F) / range, 1),
		vec4(0,0,2* F * N / range, 0)
		);
	vec4 vertex(x, y, z, 1);

	vec4 vertex2 = projMatrix * vertex;
	vec4 vertex3 = viewMatrix * vertex2;

	pntx = vertex3.x;
	pnty = vertex3.y;
}

    // Hints: V = identity, P = identity, and just get the 
    // world window to viewport working. This will produce
    // a simple 2D house: a square with a triangle on top in red.


// Hitting the 1 key sets the camera such that the y axis is up
// Hitting the 2 key sets the camera such that the 'x' axis is up
// Hitting the 3 key sets the camera such that the 'z' axis is up
// Hitting the q, Q key exits the programming
//
// DO NOT MODIFY THIS FUNCTION
//
void myKeyboard( unsigned char key, int x, int y ){
    if (key=='1'){ 
	upx=0; upy=1; upz=0;
        myDisplay();    
    }	
    if (key=='2'){
	upx=1; upy=0; upz=0;
        myDisplay();    
    }
    if (key=='3'){
	upx=0; upy=0; upz=1;
        myDisplay();    
    }
    if (key=='q' or key=='Q') exit(0);    
}

// Main function
//
// DO NOT MODIFY THIS ROUTINE.
//
int main(int argc,char ** argv){
    glutInit(&argc,argv);                         
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);    
    glutInitWindowSize((int)screenWidth,(int)screenHeight);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Three views for the Midterm");
    glutDisplayFunc(myDisplay);   
    glutKeyboardFunc(myKeyboard);
    
    
    myInit();
    glutMainLoop();
}



