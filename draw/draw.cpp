#include <math.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <iterator>
#include <GL/glut.h>

using namespace std;
int screenWidth;
int screenHeight;
std::string file;

struct point {
  double x;
  double y;
} ;

vector<vector <point> > cors;

void myInit()
{
  glClearColor(1.0,1.0,1.0,0.0);
  glColor3f(0.0f, 0.0f, 0.0f);
  glPointSize(2.0);   
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
}

void readFile(char * filename) {
  string line;
  cout << "opening " << filename << endl;
  ifstream file(filename);
  if (file == NULL) {
    cout << "Could not open file" << endl;
    exit (EXIT_FAILURE);
  }
  double x, y, x2, y2;
  file >> x;
  file >> y;
  file >> x2;
  file >> y2;
  screenWidth = x2;
  screenHeight = y;
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Drawing");
  stringstream ss;
  file >> x;
  for (int i = 0; i < x; i++) {
    vector<point> points;
    file >> y;
    for (int j = 0; j < y; j++) {
      point p;
      file >> x2;
      file >> y2;
      p.x = x2;
      p.y = y2;
      points.push_back(p);
    }
      cors.push_back(points);
  }
  file.close();
}

void myDisplay(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto & element : cors) {
    for (auto & el : element) {
      glBegin(GL_LINE_STRIP);
      glVertex2d(el.x, el.y);
    }
    glEnd();
    glFlush();
  }
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  if (argc < 2) {
    cout << "Usage: draw [filename]" << endl;
    return 1;
  } else {
    readFile(argv[1]);
  }
  myInit();
  glutDisplayFunc(myDisplay);
  glutMainLoop();
  return 0;
}
