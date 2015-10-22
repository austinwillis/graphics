#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <unistd.h>

#define screenHeight 700
#define screenWidth 700

using namespace std;

int polys = 0, click = 1;
bool userAbort = false;
const char *filename = "chamber.txt";

void myInit()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glColor3f(1.0,0.0,0.0);
  glPointSize(2.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 700.0, 0.0, 700.0);
  glViewport (0,0,700, 700);
}

struct vectorL {
  double x,y;
};

struct GLintPoint {
  GLint x,y;
};

struct segment {
  GLintPoint start;
  GLintPoint end;
  vectorL direction;
  vectorL normal;
};
segment hitWall;
segment ray, incidentRay; 
vectorL unitNormalVector, unitIncidentVector;
double normalMagnitude = 0, rayMagnitude = 0;

struct pillar {
  int numberOfSides;
  segment* segmentList;
  GLintPoint* points;
  float colorParam1, colorParam2, colorParam3;
};

pillar* polygonsList;

void drawPolyline( pillar polygon ) {
  glColor3f(polygon.colorParam1,polygon.colorParam2,polygon.colorParam3);
  glBegin(GL_POLYGON);
  glPolygonMode( GL_FRONT, GL_FILL );
  
  for(int i = 0; i < polygon.numberOfSides; i++)
    glVertex2i(polygon.points[i].x, polygon.points[i].y);
  
  glEnd(); glFlush();
}

void drawDot( GLint x, GLint y) {	
  glColor3f(0.0,0.0,0.0);
  glPointSize(1.0);
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();
}

void drawPath(GLintPoint start, GLintPoint end) {	
  glColor3f(0.0,0.0,1.0);
  glBegin(GL_LINES);
  glVertex2i(start.x, start.y);
  glVertex2i(end.x, end.y);
  glEnd();
}

double dotProduct(vectorL vect1, vectorL vect2) {
  return ((vect1.x * vect2.x)+(vect1.y * vect2.y));
}

int findHitPoint(segment& ray, pillar polygonList[]) { 
  double numeratorU, denominatorU, numeratorT, denominatorT;
  double timeU = 0.0, timeT = 0.0, hitTime = 0.0, smallestHitTime = -10; 
  vectorL tempVector;
  
  for(int i = 0; i < polys; i++) {
      for(int j = 0; j < polygonList[i].numberOfSides; j++) {	  
	  tempVector.x = ray.start.x - polygonList[i].segmentList[j].start.x; 
	  tempVector.y = ray.start.y - polygonList[i].segmentList[j].start.y;
	  
	  numeratorU =  - dotProduct(polygonList[i].segmentList[j].normal, tempVector);	  
	  denominatorU = dotProduct(polygonList[i].segmentList[j].normal, ray.direction);
	  
	  numeratorT= dotProduct(ray.normal, tempVector);	  
	  denominatorT = dotProduct(ray.normal, polygonList[i].segmentList[j].direction);
	  
	  if (denominatorU != 0.0) {
	      timeU = numeratorU / denominatorU;
	      timeT = numeratorT / denominatorT; 
	      if ((timeU >=0)&&(timeT >=0)&&(timeT <= 1)) {
		  if (dotProduct (polygonList[i].segmentList[j].normal, ray.direction)< 0.0) { 	  
		      hitTime = timeU; 
		      if ((hitTime <= smallestHitTime)||(smallestHitTime == -10)) {
			  smallestHitTime = hitTime;
			  hitWall.normal.x = polygonList[i].segmentList[j].normal.x;
			  hitWall.normal.y = polygonList[i].segmentList[j].normal.y;
			}
		    }
		}
	    }
	}
    }
  ray.end.x  = ray.start.x + (ray.direction.x * smallestHitTime);
  ray.end.y  = ray.start.y + (ray.direction.y * smallestHitTime);
}

void getPillars () {
  ifstream file ( filename , ifstream::in );
  
  int vertices = 0;
  bool end = false;
  
  if(!file)
    cout << "\nError While opening the pillars list file\n" << endl;

  file >> polys;
  polygonsList = new pillar[polys];
  
  while ((!file.eof()) && (end == false)) {
      for (int i=0; i< polys; i++) { 
	  file >> vertices;
	  
	  polygonsList[i].numberOfSides = vertices;
	  polygonsList[i].segmentList = new segment[vertices];
	  polygonsList[i].points  = new GLintPoint[vertices];
	  
	  file >> polygonsList[i].colorParam1 >> polygonsList[i].colorParam2 >>  polygonsList[i].colorParam3;
	  
	  for (int j=0; j<vertices; j++)
	    file >> polygonsList[i].points[j].x >> polygonsList[i].points[j].y;
	  
	  for (int j=0; j<vertices; j++) {
	      if(j == (vertices - 1)) {
		  polygonsList[i].segmentList[j].start =  polygonsList[i].points[j];
		  polygonsList[i].segmentList[j].end =  polygonsList[i].points[0];
		} 
	      else {
		  polygonsList[i].segmentList[j].start =  polygonsList[i].points[j];
		  polygonsList[i].segmentList[j].end =  polygonsList[i].points[j+1];
		} 
	    }
	  
	  for (int j=0; j<vertices; j++) {
	      polygonsList[i].segmentList[j].direction.x = polygonsList[i].segmentList[j].end.x - polygonsList[i].segmentList[j].start.x;
	      polygonsList[i].segmentList[j].direction.y = polygonsList[i].segmentList[j].end.y - polygonsList[i].segmentList[j].start.y;
	      
	      if (i==0) {
		  polygonsList[i].segmentList[j].normal.x = - polygonsList[i].segmentList[j].direction.y;
		  polygonsList[i].segmentList[j].normal.y = polygonsList[i].segmentList[j].direction.x;
		}
	      else {
		  polygonsList[i].segmentList[j].normal.x = polygonsList[i].segmentList[j].direction.y;
		  polygonsList[i].segmentList[j].normal.y = - polygonsList[i].segmentList[j].direction.x;
		}
	    }
	}
      end = true;
    }
}

void display_chamber() {
  myInit();
  glClear(GL_COLOR_BUFFER_BIT);
  
  getPillars ();
  
  for (int i=0; i<polys; i++)
    drawPolyline(polygonsList[i]);
  
  glEnd(); glFlush();    
}

void reset() {
  click = 1;
  polys = 0;
  userAbort = false;
  display_chamber();
}

void myMouse( int button, int state, int x, int y ) { 
  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
      if (click == 1)
	{
	  ray.start.x = x;
	  ray.start.y = screenHeight-y;
	  drawDot(ray.start.x, ray.start.y);
	}
      else if (click == 2)
	{ 
	  ray.end.x = x;
	  ray.end.y = screenHeight-y;
	  drawDot(ray.end.x, ray.end.y);
	  
	  ray.direction.x = ray.end.x - ray.start.x; 
          ray.direction.y = ray.end.y - ray.start.y;  
	  ray.normal.x = - ray.direction.y; 
	  ray.normal.y = ray.direction.x; 
	  
	  while (userAbort == false)
	    {
	      usleep(50000);
	      glFlush();
	      
	      findHitPoint(ray, polygonsList);
	      
	      drawPath(ray.start, ray.end);
	      
	      incidentRay.direction.x = ray.end.x - ray.start.x; 
	      incidentRay.direction.y = ray.end.y - ray.start.y;
	      
	      normalMagnitude = sqrt (pow(hitWall.normal.x,2)+ pow(hitWall.normal.y,2)); 
	      unitNormalVector.x = hitWall.normal.x / normalMagnitude;
	      unitNormalVector.y =  hitWall.normal.y / normalMagnitude;
	      
	      rayMagnitude = sqrt (pow(incidentRay.direction.x,2)+ pow(incidentRay.direction.y,2)); 
	      unitIncidentVector.x =  incidentRay.direction.x / rayMagnitude;
	      unitIncidentVector.y =  incidentRay.direction.y / rayMagnitude;
	      
	      ray.direction.x = unitIncidentVector.x - ( 2 * dotProduct(unitIncidentVector, unitNormalVector) * unitNormalVector.x );
	      ray.direction.y = unitIncidentVector.y - ( 2 * dotProduct(unitIncidentVector, unitNormalVector) * unitNormalVector.y );
	      
	      ray.normal.x = - ray.direction.y; 
	      ray.normal.y = ray.direction.x; 
	      
	      ray.start = ray.end;
	    }
	}
      click ++;
    }
}

void myKeyboard ( unsigned char key, int mouseX, int mouseY ) {
  switch( key )
    {
    case 'r':
      reset();
      userAbort = true;
      break;
    case 'c':
      userAbort = true;	
      exit ( -1 );
    default :
      break;
    }  
}

int main(int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(screenHeight,screenWidth);
  glutInitWindowPosition(100,150);
  glutCreateWindow("Reflections in a chamber");
  
  glutMouseFunc( myMouse );
  glutKeyboardFunc( myKeyboard );
  glutDisplayFunc(display_chamber);
  
  myInit();
  glutMainLoop();
  
  return 0;
} 
