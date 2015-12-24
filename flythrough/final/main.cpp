#include <unistd.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "sdl.h"
#include "scene.h"
#include "camera.h"

Camera cam;
Scene scn;
char filename [80];

void displaySDL(void);

void move(int iter, int mode)
{
  for (int i =0; i < iter; i++)
    { 
      switch(mode) // Switch on the user's input
	{
	case 0:
	  cam.slide(0,0,-0.4); // In "w"
	  break;
        case 1:
	  cam.slide(0,0,0.4); // Out "z"
	  break;
	case 2:
	  cam.pitch(-7.0);  // Up
	  break;
	case 3:
	  cam.pitch(7.0); // Down
	  break;
	case 4:
	  cam.yaw(-7.0); // Right
	  break;
	case 5:
	  cam.yaw(7.0); // Left
	  break;
        case 6:
	  cam.roll(4.0); // roll clockwise 'a'
	  break;
	case 7:
	  cam.roll(-4.0); // roll clockwise 's'
	  break;
	}  
      sleep (.95); displaySDL(); glFlush();
    }
}

void flyThrough()
{
  cam.set(10, 4, 10, 0, 0.25, 0, 0,3,0); displaySDL(); glFlush();
  sleep(2);
  move(10, 4);
  move(10, 0);
  move(10, 5);
  move(20, 0);
  move(2, 2);
  move(4, 5);
  move(1,6);
  sleep(2);
  move(10,0);
  sleep(2);
  move(7, 4);
  move(2,3);
  sleep(2);
  move(2,2);
  move(12,5);
  move(5,0);
  sleep(2);
  move(5,3);
  move(4, 0);
  move(2,4);
  move(3,7);
  sleep(2);
  move(6,0);
  move(12,4);
  move(7,7);
  move(4,3);
  sleep(2);
  move(6,0);
  move(12,4);
  move(4,7);
  move(7,4);
  move(2,7);
  move(2,3);
  sleep(2);
  move(8,0);
  move(5,5);
  move(3,6);
  sleep(2);
  move(5,0);
  move(13,4);
  move(3,3);
  move(1,1);
  sleep(100);
}

void mySpecialKeys(int key, int x, int y)
{
  switch(key)
    {	
    case GLUT_KEY_UP:  cam.pitch(7.0); break; // pitch camera down
    case GLUT_KEY_DOWN: cam.pitch(-7.0); break; // pitch camera up	
    case GLUT_KEY_RIGHT: cam.yaw(-7.0); break; // yaw camera rigth
    case GLUT_KEY_LEFT: cam.yaw( 7.0); break;  // yaw camera left
    case GLUT_KEY_F1: exit(0);
    }
  glutPostRedisplay(); // draw it again
}


void myKeyboard(unsigned char key, int x, int y)
{
  
  switch(key) {	
    case 'w': cam.slide(0,0,-0.4); break; // slide camera forward
    case 's': cam.slide(0,0,0.4); break; // slide camera back
    case 'q': cam.roll(-4.0); break; // roll counter-clockwise
    case 'e': cam.roll(4.0); break; // roll clockwise
    case 'f': case 'G': flyThrough();
    case 'p': exit(0);
    default : break;
  }
  glutPostRedisplay(); // draw it again
}

void displaySDL(void)
{
  cam.setShape(50.0, 64.0f/48.0f, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  cam.setModelViewMatrix();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scn.drawSceneOpenGL(); glutSwapBuffers();
}

int main(int argc, char **argv)
{
  printf("\nPlease enter the SDL file name: ");
  scanf("%s", filename);
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("SDL Flythrough");
  glutSpecialFunc(mySpecialKeys);
  glutKeyboardFunc(myKeyboard);
  glutDisplayFunc(displaySDL);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glViewport(0, 0, 640, 480);
  scn.read(filename);
  scn.makeLightsOpenGL();
  
  cam.set(6, 1.3, 6, 0, 0.25, 0, 0,3,0);
  
  glutMainLoop();
}


