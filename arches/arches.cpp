//Austin Willis
#include <math.h>
#include <GL/glut.h>
#include <iostream>
const int screenWidth = 1000;
const int screenHeight = 1000;
using namespace std;

char mode;

void myInit(void)
{
  glClearColor(1.0,1.0,1.0,0.0);
  glColor3f(0.0f, 0.0f, 0.0f);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void drawArc(float cx, float cy, float r, float starta, float enda, float density){
  float angle_delta = enda - starta;
  float angle_step = angle_delta / (density - 1);
  float angle = starta;
  glBegin(GL_LINE_STRIP);
  while(density){
    float cx2 = cx + (cos(angle) * r);
    float cy2 = cy + (sin(angle) * r);
    glVertex2d(cx2, cy2); 
    angle += angle_step;
    --density;
  }
  glEnd();
} 

void drawlines() {
  glBegin(GL_LINE_STRIP);
  glVertex2d(screenWidth/2-350,screenHeight/2);
  glVertex2d(screenWidth/2-350,screenHeight/2-400);
  glVertex2d(screenWidth/2+400,screenHeight/2-400);
  glVertex2d(screenWidth/2+400,screenHeight/2);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex2d(screenWidth/2-250, screenHeight/2);
  glVertex2d(screenWidth/2-250, screenHeight/2-250);
  glVertex2d(screenWidth/2-100, screenHeight/2-250);
  glVertex2d(screenWidth/2-100, screenHeight/2);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex2d(screenWidth/2+100, screenHeight/2);
  glVertex2d(screenWidth/2+100, screenHeight/2-250);
  glVertex2d(screenWidth/2+200, screenHeight/2-250);
  glVertex2d(screenWidth/2+200, screenHeight/2);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex2d(screenWidth/2-50, screenHeight/2);
  glVertex2d(screenWidth/2+50, screenHeight/2);
  glEnd();
}


void  drawSmoothArc(float x, float y, float radius) {
  drawArc(x, y, radius, 0, M_PI, 100);
}

void drawPointedArc(float x, float y, float radius) {
  drawArc(x-radius/2, y, radius, 0, M_PI/3, 100);
  drawArc(x+radius/2, y, radius, M_PI, M_PI*2/3, 100);
}

void drawKeelArc(float x, float y, float radius) {
    drawArc(x, y, radius, 0, M_PI/3, 100);
    drawArc(x+radius, y+radius*1.74, radius, -M_PI, -M_PI*2/3, 100);
    drawArc(x-radius, y+radius*1.74, radius, 0, -M_PI/3, 100);
    drawArc(x, y, radius, M_PI, M_PI*2/3, 100);
}

void keyboard(unsigned char key, int x, int y) {
  mode = key;
  glutPostRedisplay();
}

void myDisplay(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (mode == 'a') {
    drawSmoothArc(screenWidth/2, screenHeight/2, 400);
  }
  if (mode == 'p') {
    drawPointedArc(screenWidth/2, screenHeight/2, 300);
  }
  if (mode == 'k') {
    drawKeelArc(screenWidth/2, screenHeight/2, 200);
  }
  if (mode == 'd') {
    drawKeelArc(screenWidth/2-300, screenHeight/2, 50);
    drawPointedArc(screenWidth/2, screenHeight/2, 200);
    drawPointedArc(screenWidth/2, screenHeight/2, 100);
    drawSmoothArc(screenWidth/2+300, screenHeight/2, 100);
    drawlines();
  }
  glFlush();
}

int main(int argc, char* argv[])
{
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
      glutInitWindowSize(screenWidth, screenHeight);
      glutInitWindowPosition(100, 150);
      glutCreateWindow("Arches");
      glutDisplayFunc(myDisplay);
      glutKeyboardFunc(keyboard);
      myInit();
      cout << "Keyboard commands:" << endl;
      cout << "a = arch" << endl;
      cout << "p = pointed arch" << endl;
      cout << "k = keel arch" << endl;
      cout << "d = draw a picture" << endl;
      glutMainLoop();
  return 0;
}
