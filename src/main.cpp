#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

double RADIUS = 50;
double YZ_ANGLE = M_PI / 100;
double ZX_ANGLE = M_PI / 100;

void init();
void display();
void OnKeyboard(int, int, int);
void onReshape(int width, int height) {}
void setCameraPosition();
double getXEye();
double getYEye();
double getZEye();

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 10);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Lab3");
  glutReshapeFunc(onReshape);
  init();
  glutSpecialFunc(OnKeyboard);
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutMainLoop();
  return 0;
}

void init() {
  // glEnable(GL_COLOR_MATERIAL);
  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);
  // glEnable(GL_DEPTH_TEST);
  // glClearColor(0.0, 0.0, 0.0, 0.0);
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // glOrtho(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2,
  // WINDOW_HEIGHT / 2, 2, 12);
  // gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
  // glMatrixMode(GL_MODELVIEW);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 2, 100);
  setCameraPosition();
  glMatrixMode(GL_MODELVIEW);

  glColor3d(255, 130, 255);
  glLineWidth(10);

  glPointSize(1);

  double R = 5;

  glBegin(GL_POINTS);

  for (double B = 0; B < 2 * M_PI; B += M_PI / 10) {
    for (double L = 0; L < 2 * M_PI; L += M_PI / 10) {
      double x = R * cos(B) * sin(L);
      double y = R * cos(B) * cos(L);
      double z = R * sin(B);

      glVertex3d(x, y, z);
    }
  }

  glEnd();

  glutSwapBuffers();
}

void setCameraPosition() {
  double a = cos(YZ_ANGLE);
  gluLookAt(getXEye(), getYEye(), getZEye(), 0, 0, 0, 0,
            YZ_ANGLE < 0 || YZ_ANGLE > M_PI ? -1 : 1, 0);
}

double getXEye() { return RADIUS * sin(YZ_ANGLE) * sin(ZX_ANGLE); }

double getYEye() { return RADIUS * cos(YZ_ANGLE); }

double getZEye() { return RADIUS * sin(YZ_ANGLE) * cos(ZX_ANGLE); }

void OnKeyboard(int key, int x, int y) {

  if (key == GLUT_KEY_UP) {
    printf("up");
    YZ_ANGLE -= M_PI / 10;
  }

  if (key == GLUT_KEY_DOWN) {
    printf("down");
    YZ_ANGLE += M_PI / 10;
  }

  if (key == GLUT_KEY_LEFT) {
    printf("left");
    ZX_ANGLE -= M_PI / 10;
  }

  if (key == GLUT_KEY_RIGHT) {
    printf("right");
    ZX_ANGLE += M_PI / 10;
  }

  if (YZ_ANGLE > M_PI) {
    YZ_ANGLE = -M_PI;
  }

  if (YZ_ANGLE < -M_PI) {
    YZ_ANGLE = M_PI;
  }

  if (YZ_ANGLE == 0) {
    YZ_ANGLE -= 0.001;
  }

  if (ZX_ANGLE > M_PI * 2) {
    ZX_ANGLE -= M_PI * 2;
  }

  if (ZX_ANGLE < -M_PI * 2) {
    ZX_ANGLE *= -1;
  }
}
