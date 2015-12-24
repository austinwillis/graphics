#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "GlutWin.h"
#include "Camera.h"
#include "Vector3.h"

using namespace std;

GlutWin		*win;
Camera		*camera;

// mouse cursor position
float		mouse_x,
			mouse_y;
int 		mode = 1;
bool		mouse_updated = false;

float lastcx;
float lastcy;

int num_steps_u = 30;
int num_steps_v = 15;

// torus values
float		r_maj = 5.0; // distance from origin to face
float		r_min = 2.0; // radius of a meridian

Vector3 p( float u, float v ) {

	// return a point on the torus for u,v both on [0, 2pi]
	// where v = radians around a meridian
	// u = radians "swept" around the axis

	return( Vector3( cos(u)*(r_maj+r_min*cos(v)),
					 sin(u)*(r_maj+r_min*cos(v)),
					 r_min*sin(v) ) );


}

Vector3 n( float u, float v ) {
	
	Vector3 N;

	N.x = cos(v)*cos(u);
	N.y = cos(v)*sin(u);
	N.z = sin(v);

	N.normalize();

	return( N );

}

bool initdemo() {

	// initialize GLUT class
	win = new GlutWin( 600, 800,
						100, 100,
						GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH,
						"Final" );


	camera = new Camera();
	camera->set( 5, 5, 5, 0, 5, 5, 0, 1, 0 );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	// enable depth testing
	glEnable( GL_DEPTH_TEST );

	// set up lighting in ogl
	glEnable( GL_LIGHTING );

	float light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
	float light_dif[] = { 0.25, 0.25, 0.25, 1.0 };
	float light_pos[] = { 10.0, 10.0, 0.0, 1.0 };

	float material_amb[] = { 1.0, 1.0, 1.0, 1.0 };
	float material_dif[] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, material_amb );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, material_dif );

	glLightfv( GL_LIGHT0, GL_AMBIENT, light_amb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_dif );

	glLightfv( GL_LIGHT0, GL_POSITION, light_pos );

	glEnable( GL_LIGHT0 );

	return( true );

}

void drawArc(float cx, float cy, float r, float starta, float enda, float density){
  float angle_delta = enda - starta;
  float angle_step = angle_delta / (density - 1);
  float angle = starta;
  float cx2 = cx + (cos(angle) * r);
  float cy2 = cy + (sin(angle) * r);
  lastcx = cx2;
  lastcy = cy2;
  angle += angle_step;
  --density;
   while(density > 0){
    cx2 = cx2 + (cos(angle) * r);
    cy2 = cy2 + (sin(angle) * r);
    
    glVertex3d(cx2, 0, cy2);
    glVertex3d(lastcx, 0, lastcy); 
    glVertex3d(0, lastcx, lastcy);
    glVertex3d(0, cx2, cy2);
    
    glVertex3d(-cx2, 0, cy2);
    glVertex3d(-lastcx, 0, lastcy); 
    glVertex3d(0, lastcx, lastcy);
    glVertex3d(0, cx2, cy2);
    
    glVertex3d(cx2, 0, cy2);
    glVertex3d(lastcx, 0, lastcy); 
    glVertex3d(0, -lastcx, lastcy);
    glVertex3d(0, -cx2, cy2);
    
    glVertex3d(-cx2, 0, cy2);
    glVertex3d(-lastcx, 0, lastcy); 
    glVertex3d(0, -lastcx, lastcy);
    glVertex3d(0, -cx2, cy2);
    
    angle += angle_step;
    density--;
   }
}

void  drawSmoothArc(float x, float y, float radius) {
  drawArc(x, y, radius, 0, M_PI/2, 100);
}

void drawKeelArc(float x, float y, float radius) {
    drawArc(x, y, radius, 0, M_PI/3, 5);
    drawArc(x+radius, y+radius*1.74, radius, -M_PI, -M_PI*2/3, 5);
    //drawArc(x-radius, y+radius*1.74, radius, 0, -M_PI/3, 15);
    //drawArc(x, y, radius, M_PI, M_PI*2/3, 15);
}

void render() {

	const float PI = 3.14159265359;

	float step_u = (2*PI)/(float)num_steps_u;
	float step_v = (2*PI)/(float)num_steps_v;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	glBegin( GL_QUADS );

	 // for( float u = 0; u < 1 * step_u; u += step_u ) {
	 // 	for( float v = 0; v < 1 * step_v; v += step_v ) {

	 // 		Vector3 P[4], N[4];
	
	 // 		P[0] = p( u, v );
	 // 		P[1] = p( u+step_u, v );
	 // 		P[2] = p( u+step_u, v+step_v );
	 // 		P[3] = p( u, v+step_v );

	 // 		N[0] = n( u, v );
	 // 		N[1] = n( u+step_u, v );
	 // 		N[2] = n( u+step_u, v+step_v );
	 // 		N[3] = n( u, v+step_v ); 

	 // 		for( int c = 0; c < 4; c++ ) {
	 // 			glNormal3f( N[c].x, N[c].y, N[c].z );
	 // 			glVertex3f( P[c].x, P[c].y, P[c].z );
	 // 		}
	 // 	}
	 // }

	if (mode == 1)
		drawSmoothArc(0,0,5);
	if (mode == 2)
		drawKeelArc(0,0,.0005);

	glEnd();


	// refresh image
	glutSwapBuffers();
	glutPostRedisplay();


}


void kb_input( unsigned char key, int x, int y ) {


	switch( key ) {

	case 'a':	camera->slide( -1.0, 0.0, 0.0 ); break;
	case 'd':	camera->slide( 1.0, 0.0, 0.0 ); break;
	case 's':	camera->slide( 0.0, 0.0, 1.0 ); break;
	case 'w':	camera->slide( 0.0, 0.0, -1.0 ); break;
	case '+':	if( r_maj < 10) r_maj+=1.0; break;
	case '-':	if( r_maj > 3 ) r_maj-=1.0; break;
	case '/':	if( r_min < 5) r_min+=1.0; break;
	case '*':	if( r_min > 1 ) r_min-=1.0; break;
	case 'q':	exit( 0 );
	default:	break;

	}

	glutPostRedisplay();
}

void mouse_motion( int x, int y ) {

	float dx, dy;

	if( ! mouse_updated) {

		mouse_x = x;
		mouse_y = y;

		mouse_updated = true;

	} else {

		// calc delta
		dx = x - mouse_x;
		dy = y - mouse_y;

		mouse_x = x;
		mouse_y = y;

		// determine rotation amount

		camera->pitch( dy / 5.0 );
		camera->yaw( dx );


	}

}

int main(int argc, char** argv) {

	if (argc > 1)
		mode = atoi(argv[1]);
	// init
	if( initdemo() ) {

		glutDisplayFunc( render );
		glutKeyboardFunc( kb_input );
		glutPassiveMotionFunc( mouse_motion );

		glutMainLoop();

		delete win;
		delete camera;

	}
	return 0;

}











	
