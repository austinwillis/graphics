// build with
// g++ -o function function.cpp -L. -lGL -lglut -lGLU
#include <math.h>
#include <GL/glut.h>
const int screenWidth = 640;
const int screenHeight = 480;
GLdouble A, B, C, D;

void myInit(void)
{
  glClearColor(1.0,1.0,1.0,0.0);
  glColor3f(0.0f, 0.0f, 0.0f);
  glPointSize(2.0);   glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
  A = screenWidth / 4.0;
  B = 0.0;
  C = D = screenHeight / 2.0;
}

void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  for(GLdouble x = 0; x < 4.0; x += .0005)
  {
    GLdouble func = exp(-x) * cos(2 * 3.14159265 * x);
    glVertex2d(A * x + B, C * func + D);
  }
  glEnd();
  glFlush();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(screenWidth, screenHeight);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Dot Plot of a Function");
  glutDisplayFunc(myDisplay);
  myInit();
  glutMainLoop();
  return 0;
}
