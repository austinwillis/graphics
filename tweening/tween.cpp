#include <GL/glut.h>
#include <iostream>

#define RATIO 1.618
#define WW    100
#define WH    (WW/RATIO)
#define HALFX ((int)(WW/2))
#define HALFY ((int)(WH/2))
int WindowWidth;
int WindowHeight;
void InitGL();
void Display();
void Reshape(int,int);
void Keyboard(unsigned char key, int x, int y);
void drawTween(float StartShape[9][2], float EndShape[9][2]);
int mode = 1;
float deltat = .005;
float Tween=0.0-deltat;

using namespace std;

int main(int argc, char **argv)
{

    glutInit(&argc,argv);   
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

    WindowWidth  = (int)(glutGet((GLenum)GLUT_SCREEN_WIDTH)*.8); 
    WindowHeight = (int)(WindowWidth/RATIO);

    glutInitWindowSize(WindowWidth,WindowHeight); 
    glutInitWindowPosition((int)(glutGet((GLenum)GLUT_SCREEN_WIDTH)*.1),(glutGet((GLenum)GLUT_SCREEN_HEIGHT)/2)-(WindowHeight/2));
    glutCreateWindow("Tweening");

    glutKeyboardFunc(Keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    InitGL();
    cout << "Keyboard commands" << endl;
    cout << "f = draw from house to t" << endl;
    cout << "r = draw from t to house" << endl;
    cout << "q = quit" << endl;
    cout << "s = increase speed" << endl;
    cout << "d = decrease speed" << endl;
    glutMainLoop();

    return 0;
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == 'f' && (mode == 2 || mode == 0)) {
	mode = 1;
	Tween = 1 - Tween;
    }
    if (key == 'r' && (mode == 1 || mode == 0)) {
	mode = 2;
        Tween = 1 - Tween;
    }
    if (key == 's' && (mode == 1 || mode == 2)) {
	deltat += .001;
    }
    if (key == 'd' && (mode == 1 || mode == 2)) {
	deltat -= .001;
        if (deltat < .002) {
	    deltat = .003;
        }
    }
    if (key == 'q' && (mode == 1 || mode == 2)) {
	exit(0);
    }
}

void Display()
{ 
    float EndShape[9][2]={{0,10},{-25,10},{-25,0},{-5,0},{-5,-20},{5,-20},{5,0},{25,0},{25,10}};
    float StartShape[9][2]={{-25,10},{-25,-20},{-5,-20},{-5,-10},{5,-10},{5,-20},{25,-20},{25,10},{0,20}};
    if (mode == 1) {
	drawTween(StartShape, EndShape);
    }
    if (mode == 2) {
	drawTween(EndShape, StartShape);
    }
    glutPostRedisplay();
}

void drawTween(float StartShape[9][2], float EndShape[9][2]) {
    float IntermediateShape[9][2];
    float VertexColors[9][3]={{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
    if(Tween<1)
    {
        Tween+= deltat;
    }
    for (int Vtx=0;Vtx<9;Vtx++)
    {
        IntermediateShape[Vtx][0]=(1.0-Tween)*StartShape[Vtx][0]+Tween*EndShape[Vtx][0];
        IntermediateShape[Vtx][1]=(1.0-Tween)*StartShape[Vtx][1]+Tween*EndShape[Vtx][1];
    }
    glVertexPointer(2,GL_FLOAT,0,IntermediateShape);
    glColorPointer(3,GL_FLOAT,0,VertexColors);
	for (int i = 0; i < 1000000; i++);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_LINE_LOOP,0,9);

    glutSwapBuffers();
    glutPostRedisplay();
}

void Reshape(int w,int h)
{
    glutReshapeWindow(w,(int)(w/RATIO));
    WindowWidth=w;
    WindowHeight=(int)(w/RATIO);
    InitGL();
}

void InitGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-HALFX,HALFX,-HALFY,HALFY);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1, 1, 1, 1);
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_COLOR_ARRAY);
    glShadeModel(GL_SMOOTH);
    glViewport(0,0,WindowWidth,WindowHeight);
}
