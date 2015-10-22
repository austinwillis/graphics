#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>

#define screenHeight 700
#define screenWidth 700

using namespace std;
char mode;
int click = 1;
int startpoly = 0;

void myInit() {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0,0.0,0.0);
  glPointSize(2.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,700.0,0.0,700.0);
  glViewport(0,0,700,700);
}

struct vec {
  double x, y;
};

struct Point2 {
  GLint x, y;
};

Point2 initp;
Point2 lastp;

vector<Point2> currentpts;

struct segment {
  Point2 start;
  Point2 end;
  vec direction;
  vec normal;
};

vector<segment> segments;

struct poly {
  GLfloat c1, c2, c3;
  vector<Point2> points;
};

void makeSegmentsfromPoly(poly);
vector<poly> polygons;
segment hitwall;
segment ray, incident;
vec unitnormal, unitincident;
double normalmag = 0, raymag = 0;

void drawpolygon( poly p )
{
  glColor3f(p.c1, p.c2, p.c3);
  glBegin(GL_POLYGON);
  glPolygonMode(GL_FRONT, GL_FILL);
  for (auto& it : p.points) {
    glVertex2i(it.x, it.y);
  }
  glEnd();
  glFlush();
}

void drawcurrentpoly() {
  poly p;
  p.c1 = 0.0;
  p.c2 = 0.0;
  p.c3 = 0.0;
  for (auto& it : currentpts) {
    p.points.push_back(it);
  }
  polygons.push_back(p);
  currentpts.clear();
  glutPostRedisplay();
  segment s;
  s.start = lastp;
  s.end = initp;
  s.direction.x = lastp.x - initp.x;
  s.direction.y = lastp.y - initp.y;
  s.normal.x = - lastp.y;
  s.normal.y = lastp.x;
  segments.push_back(s);
} 

vec makedirection(Point2 p1, Point2 p2) {
  vec v;
  v.x = p2.x - p1.x;
  v.y = p2.y - p1.y;
  return v;
}

vec makenormal(segment s) {
  vec v;
  v.x = s.direction.y * -1;
  v.y = s.direction.x;
  return v;
}

void drawDot( Point2 p )
{
  glColor3f(1.0,0.0,1.0);
  glPointSize(3.0);
  glBegin(GL_POINTS);
  glVertex2i(p.x, p.y);
  glEnd();
  glFlush();
}

void drawLine( Point2 start, Point2 end )
{
  glColor3f(0.0,0.0,1.0);
  glBegin(GL_LINES);
  glVertex2i(start.x, start.y);
  glVertex2i(end.x, end.y);
  glEnd();
  glFlush();
}

double dot( vec v1, vec v2 )
{
  return ((v1.x * v2.x) + (v1.y * v2.y));
}

Point2 findHitPoint() {
  double numeratorU=0, denominatorU=0, numeratorT=0, denominatorT=0;
  double timeU = 0.0, timeT = 0.0, hitTime = 0.0, smallestHitTime = -10;
  vec tempVector;
    for(auto& it2 : segments) {
      tempVector.x = ray.start.x - it2.start.x;
      tempVector.y = ray.start.y - it2.start.y;

      numeratorU = - dot(it2.normal, tempVector);
      denominatorU = dot(it2.normal, ray.direction);

      numeratorT = dot(ray.normal, tempVector);
      denominatorT = dot(ray.normal, it2.direction);

      if (denominatorU != 0.0) {
        timeU = numeratorU / denominatorU;
        timeT = numeratorT / denominatorT;
        
    	if ((timeU >= 0) && (timeT >= 0) && (timeT <= 1)) {
   	  if (dot(it2.normal, ray.direction) < 0.0) {
            hitTime = timeU;
		
 	    if ((hitTime <= smallestHitTime) 
                 || (smallestHitTime == -10)) {
	      smallestHitTime = hitTime;

              hitwall.normal.x = it2.normal.x;
              hitwall.normal.y = it2.normal.y;
          }
        }
      }
    }
  }
  Point2 p;
  p.x = ray.start.x + (ray.direction.x * smallestHitTime);
  p.y = ray.start.y + (ray.direction.y * smallestHitTime);
  return p;
}

void draw_chamber()
{
  myInit();
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0,0.0,1.0);
  for (auto& it: polygons) {
    drawpolygon(it);
  }
  glEnd();
  glFlush();
};

void drawPath(Point2 start, Point2 end) {
  glColor3f(0.0,0.0,0.0);
  glBegin(GL_LINES);
  glVertex2i(start.x, start.y);
  glVertex2i(end.x, end.y);
  glEnd();
}

void myMouse (int button, int state, int x, int y) {
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
  if (mode == 'n') {
    Point2 p;
    p.x = x;
    p.y = screenHeight - y;
    drawDot(p);
    currentpts.push_back(p);
    if (startpoly == 0) {
      initp = p;
      lastp = p;
      startpoly = 1;
    } else {
      segment s;
      s.start = lastp;
      s.end = p;
      s.direction.x = s.start.x - s.end.x;
      s.direction.y = s.start.y - s.end.y;
      s.normal.x = - s.direction.y;
      s.normal.y = s.direction.x;
      segments.push_back(s);
      lastp = p;
    }
  }
  if (mode == 'd') {
   if (click == 1 ) {
     ray.start.x = x;
     ray.start.y = screenHeight-y;
     Point2 p;
     p.x = x;
     p.y = screenHeight - y;
     drawDot(p);
     click = 2;
   } else if (click == 2) {
     ray.end.x = x;
     ray.end.y = screenHeight - y;
     ray.direction.x = ray.start.x - ray.end.x;
     ray.direction.y = ray.start.y - ray.end.y;
     ray.normal.x = - ray.direction.y;
     ray.normal.y = ray.direction.x;
     int i = 0;
     while (i < 6) {
       glFlush();
       ray.end = findHitPoint();
       drawPath(ray.start, ray.end);    
       usleep(100000);
       incident.direction.x = ray.end.x - ray.start.x;
       incident.direction.y = ray.end.y - ray.start.y;

       normalmag = sqrt (pow(hitwall.normal.x,2) + pow(hitwall.normal.y, 2));
       unitnormal.x = hitwall.normal.x / normalmag;
       unitnormal.y = hitwall.normal.y / normalmag;

       raymag = sqrt(pow(incident.direction.x,2)+ pow(incident.direction.y,2));
       unitincident.x = incident.direction.x / raymag;
       unitincident.y = incident.direction.y / raymag;

       ray.direction.x = unitincident.x = (2 * dot(unitincident,unitnormal) * unitnormal.x);
       ray.direction.y = unitincident.y = (2 * dot(unitincident,unitnormal) * unitnormal.y);

       ray.normal.x = - ray.direction.y;
       ray.normal.y = ray.direction.x;

       ray.start = ray.end;
       i++;
     }
     click = 1;
   }
  }
 }
}

void myKeyboard (unsigned char key, int mouseX, int mouseY) {
  switch(key) {
    case 'd':
      mode = key;
      break;
    case 'r':
      cout << "Reset" << endl;
      mode = key;
      break;
    case 'n':
      cout << "Started drawing a new polygon" << endl;
      mode = key;
      break;
    case 'e':
      drawcurrentpoly();
      cout << "Drew polygon" << endl;
      mode = key;
      break;
    case 'q':
      exit(1);
    default:
      break;
  }
}

int main(int argc, char ** argv) {
  ray.end.x = 0;
  ray.end.y = 0;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(screenHeight, screenWidth);
  glutInitWindowPosition(100, 150);
  glutCreateWindow("Reflection Chamber");
  glutMouseFunc(myMouse);
  glutKeyboardFunc(myKeyboard);

  poly pol;
  Point2 p;
  p.x = 50;
  p.y = 50;
  pol.points.push_back(p);
  p.y = 650;
  pol.points.push_back(p);
  p.x = 650;
  pol.points.push_back(p);
  p.y = 50;
  pol.points.push_back(p);
  pol.c1 = 0.0;
  pol.c2 = 1.0;
  pol.c3 = 1.0;
  polygons.push_back(pol);

  glutDisplayFunc(draw_chamber);
  cout << "Keyboard Functions" << endl;
  cout << "    d : draw a ray" << endl;
  cout << "    n : start drawing a new polygon" << endl;
  cout << "    e : finish drawing the polygon" << endl;
  cout << "    q : quit the application" << endl;
  cout << "    r : reset the chamber" << endl;
  

  myInit();
  glutMainLoop();

  return 0;
}
