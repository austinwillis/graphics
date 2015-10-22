
// This will pull in all GL and GLU declarations
#include <GL/glut.h>

#include <iostream>

// Call back routines prototypes
void myDisplay(); 
void myKeyboard(unsigned char, int, int);
void myMouse(int,int,int,int);

// Initialization routine
void myInit();

// Main program
int main(int argc, char **argv)
{


  GLbyte a;
  GLshort b;
  GLint c;
  GLfloat d;
  GLdouble e;
  GLubyte f;
  GLushort g;
  GLuint h;

  std::cout << sizeof(a) << std::endl;
  std::cout << sizeof(b) << std::endl;
  std::cout << sizeof(c) << std::endl;
  std::cout << sizeof(d) << std::endl;
  std::cout << sizeof(e) << std::endl;
  std::cout << sizeof(f) << std::endl;
  std::cout << sizeof(g) << std::endl;
  std::cout << sizeof(h) << std::endl;




  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);  

  glutInitWindowSize(640,480);
  glutInitWindowPosition(100,150);
  glutCreateWindow("First Programming Attempt");


  // Register my call back functions
  glutDisplayFunc(myDisplay);
  glutKeyboardFunc(myKeyboard);
  glutMouseFunc(myMouse);
  
  myInit();
  glutMainLoop();

  return 0;  // Should never get here
}

// Call back implementation
void myDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_POINTS);
     glVertex2i(0100,0050);
  glEnd();
  glFlush();
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
   exit(0);
}

void myMouse(int button, int state, int x, int y)
{
  exit(0);
}

void myInit(){
    glClearColor(1.0,1.0,1.0,0.0);      // choose background color
    glColor3f(1.0f, 0.0f, 0.0f);        // set the drawing color
    glPointSize(4.0);                   // set point size
    glMatrixMode(GL_PROJECTION);        // load matrix mode
    glLoadIdentity();                   // load identity matrix
    gluOrtho2D(0.0, 640.0, 0.0, 0480.0); // defines a 2D orthographic projection
}

