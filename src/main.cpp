#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

double RADIUS = 30;
double YZ_ANGLE = M_PI / 100;
double ZX_ANGLE = M_PI / 100;

float LIGHT_X = 20;
float LIGHT_Y = 20;
float LIGHT_Z = 20;

int MODE = 0;

GLuint TEXTURE;

void init();
void display();
void onKeyboard(int, int, int);
void onReshape(int width, int height) {}
void setCameraPosition();
double getXEye();
double getYEye();
double getZEye();
void DrawFrame(int radius, int deltaLatitude, int deltaLongitude);
void DrawFrameLatitue(int radius, int deltaLatitude, int deltaLongitude);
void DrawFrameLongitude(int radius, int deltaLatitude, int deltaLongitude);
void DrawPolygons(int radius, int deltaLatitude, int deltaLongitude,
                  bool useTexture);
void DrawPolygon(int radius, int latitude, int deltaLatitude, int longitude,
                 int deltaLongitude, bool useTexture, double texX, double texY,
                 double texWX, double texWY);
void OnMouseClick(int btn, int state, int x, int y);
void onKeyboard(unsigned char, int, int);
uint loadTexture(const char *filename);

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(50, 10);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("rgz Churikov Danil AVT-113");
  glutReshapeFunc(onReshape);
  init();
  TEXTURE = loadTexture("/home/danilka108/graphics_labs/rgz/build/texture.bmp");
  glutSpecialFunc(onKeyboard);
  glutKeyboardFunc(onKeyboard);
  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutMouseFunc(OnMouseClick);
  glutMainLoop();

  return 0;
}

void init() {
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display() {
  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0f, 0.4f, 0.8f);
  glPushMatrix();
  GLfloat mat_specular[] = {0.0, 0.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {50.0};

  float lpos0[] = {-5.0f + LIGHT_X, -5.0f + LIGHT_Y, -5.0f + LIGHT_Z, 1.0f};
  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, lpos0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  switch (MODE) {
  case 3: {
    gluPerspective(50, 1, 1, 400);
    setCameraPosition();
    glRotatef(20, 0, 1, 0);
    glRotatef(-27, 1, 0, 0);
    glRotatef(37, 0, 0, 1);
    break;
  }
  case 4: {
    glOrtho(-15, 15, -15, 15, 40, -40);
    setCameraPosition();
    glRotatef(45, 0, 0, 1);
    glRotatef(30, 1, 0, 0);
    break;
  }
    // case 5: {
    // break;
    //}
  default: {
    glOrtho(-15, 15, -15, 15, -40, 40);
    setCameraPosition();
    break;
  }
  }

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch (MODE) {
  case 2:
    DrawPolygons(5, 2, 2, false);
    DrawFrame(5, 10, 10);
    break;
  case 1:
  case 5:
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, TEXTURE);
    DrawPolygons(5, 2, 2, true);
    glDisable(GL_TEXTURE_2D);
    break;
  default:
    DrawFrame(5, 10, 10);
  }

  glutSwapBuffers();
}

double RAD = M_PI / 180.0;

void DrawFrame(int radius, int deltaLatitude, int deltaLongitude) {
  glColor3d(255, 130, 255);
  glLineWidth(1);

  DrawFrameLatitue(radius, deltaLatitude, deltaLongitude);
  DrawFrameLongitude(radius, deltaLatitude, deltaLongitude);
}

void DrawFrameLatitue(int radius, int deltaLatitude, int deltaLongitude) {
  for (int longitude = -90; longitude <= 90; longitude += deltaLongitude) {
    glBegin(GL_LINE_STRIP);
    for (int latitude = 0; latitude <= 360; latitude += deltaLatitude) {

      double x = radius * cos(longitude * RAD) * sin(latitude * RAD);
      double y = radius * cos(longitude * RAD) * cos(latitude * RAD);
      double z = radius * sin(longitude * RAD);

      z = z - radius * pow(longitude * RAD / (M_PI / 2), 3);

      glVertex3d(y, z, x);
    }
    glEnd();
  }
}

void DrawFrameLongitude(int radius, int deltaLatitude, int deltaLongitude) {
  for (int latitude = 0; latitude <= 360; latitude += deltaLatitude) {
    glBegin(GL_LINE_STRIP);
    for (int longitude = -90; longitude <= 90; longitude += deltaLongitude) {

      double x = radius * cos(longitude * RAD) * sin(latitude * RAD);
      double y = radius * cos(longitude * RAD) * cos(latitude * RAD);
      double z = radius * sin(longitude * RAD);

      z = z - radius * pow(longitude * RAD / (M_PI / 2), 3);

      glVertex3d(y, z, x);
    }
    glEnd();
  }
}

void DrawPolygons(int radius, int deltaLatitude, int deltaLongitude,
                  bool useTexture) {
  GLfloat texX = 0, texY = 0, texWX = 1 / (180 / deltaLongitude),
          texWY = 1 / (360 / deltaLatitude);

  for (int latitude = 0; latitude <= 360; latitude += deltaLatitude) {
    for (int longitude = -90; longitude <= 90; longitude += deltaLongitude) {
      DrawPolygon(radius, latitude, deltaLatitude, longitude, deltaLongitude,
                  useTexture, texX, texY, texWX, texWY);
      texX += texWX;
    }
    texY += texWY;
    texX = 0;
  }
}

void DrawPolygon(int radius, int latitude, int deltaLatitude, int longitude,
                 int deltaLongitude, bool useTexture, double texX, double texY,
                 double texWX, double texWY) {

  double x1 = radius * cos(longitude * RAD) * sin(latitude * RAD);
  double y1 = radius * cos(longitude * RAD) * cos(latitude * RAD);
  double z1 = radius * sin(longitude * RAD);
  z1 = z1 - radius * pow(longitude * RAD / (M_PI / 2), 3);

  double x2 =
      radius * cos((longitude + deltaLongitude) * RAD) * sin(latitude * RAD);
  double y2 =
      radius * cos((longitude + deltaLongitude) * RAD) * cos(latitude * RAD);
  double z2 = radius * sin((longitude + deltaLongitude) * RAD);
  z2 = z2 - radius * pow((longitude + deltaLongitude) * RAD / (M_PI / 2), 3);

  double x3 = radius * cos((longitude + deltaLongitude) * RAD) *
              sin((latitude + deltaLatitude) * RAD);
  double y3 = radius * cos((longitude + deltaLongitude) * RAD) *
              cos((latitude + deltaLatitude) * RAD);
  double z3 = radius * sin((longitude + deltaLongitude) * RAD);
  z3 = z3 - radius * pow((longitude + deltaLongitude) * RAD / (M_PI / 2), 3);

  double x4 =
      radius * cos(longitude * RAD) * sin((latitude + deltaLatitude) * RAD);
  double y4 =
      radius * cos(longitude * RAD) * cos((latitude + deltaLatitude) * RAD);
  double z4 = radius * sin(longitude * RAD);
  z4 = z4 - radius * pow(longitude * RAD / (M_PI / 2), 3);

  float a[3];
  float b[3];
  float m[3];

  a[0] = x2 - x1;
  a[1] = y2 - y1;
  a[2] = z2 - z1;
  b[0] = x3 - x1;
  b[1] = y3 - y1;
  b[2] = z3 - z1;
  m[0] = 10 * (a[1] * b[2] - a[2] * b[1]);
  m[1] = 10 * (a[0] * b[2] - a[2] * b[0]);
  m[2] = 10 * (a[0] * b[1] - a[1] * b[0]);

  if (!useTexture)
    glColor3d(0.8, 0.3, 0.6);
  glNormal3fv(m);

  glBegin(GL_QUADS);

  if (useTexture)
    glTexCoord2d(texX, texY);
  // glNormal3fv(m);
  glVertex3d(y1, z1, x1);

  if (useTexture)
    glTexCoord2d(texX, texY + texWY);
  // glNormal3fv(m);
  glVertex3d(y2, z2, x2);

  if (useTexture)
    glTexCoord2d(texX + texWX, texY + texWY);
  // glNormal3fv(m);
  glVertex3d(y3, z3, x3);

  if (useTexture)
    glTexCoord2d(texX + texWX, texY);
  // glNormal3fv(m);
  glVertex3d(y4, z4, x4);
  glEnd();
}

void setCameraPosition() {
  gluLookAt(getXEye(), getYEye(), getZEye(), 0, 0, 0, 0,
            YZ_ANGLE < 0 || YZ_ANGLE > M_PI ? -1 : 1, 0);
}

double getXEye() { return RADIUS * sin(YZ_ANGLE) * sin(ZX_ANGLE); }

double getYEye() { return RADIUS * cos(YZ_ANGLE); }

double getZEye() { return RADIUS * sin(YZ_ANGLE) * cos(ZX_ANGLE); }

void onKeyboard(int key, int x, int y) {

  if (key == GLUT_KEY_UP) {
    YZ_ANGLE -= M_PI / 10;
  }

  if (key == GLUT_KEY_DOWN) {
    YZ_ANGLE += M_PI / 10;
  }

  if (key == GLUT_KEY_LEFT) {
    ZX_ANGLE -= M_PI / 10;
  }

  if (key == GLUT_KEY_RIGHT) {
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

void OnMouseClick(int btn, int state, int x, int y) {
  if (state != GLUT_DOWN)
    return;

  if (btn == 3) {
    RADIUS -= RADIUS - 1 < 0 ? 0 : 1;
  }

  if (btn == 4) {
    RADIUS += 1;
  }
}

void onKeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case '0':
    MODE = 0;
    break;
  case '1':
    MODE = 1;
    break;
  case '2':
    MODE = 2;
    break;
  case '3':
    MODE = 3;
    break;
  case '4':
    MODE = 4;
    break;
  case '5':
    MODE = 5;
    break;
  }
}

uint loadTexture(const char *filename) {
  GLuint texture;
  int width, height;
  unsigned char *data;

  FILE *file;

  file = fopen(filename, "rb");

  if (file == NULL)
    return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc(width * height * 3);
  // int size = fseek(file,);
  fread(data, width * height * 3, 1, file);
  fclose(file);

  for (int i = 0; i < width * height; ++i) {
    int index = i * 3;
    unsigned char B, R;
    B = data[index];
    R = data[index + 2];

    data[index] = R;
    data[index + 2] = B;
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_NEAREST);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE,
                    data);
  free(data);

  return texture;
}
