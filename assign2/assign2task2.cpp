/**
 * COMP 4002 - Assignment 2
 * Task 1: Render a sphere at (100,10,100) using perspective projection.
 *
 * Author: Ryan Seys - 100817604
 */
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLdouble delta = 0.1;
GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = 0.0;

GLdouble rotateDelta1 = 0.1; // degrees per frame
GLdouble rotateDelta2 = 0.2; // degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 20;

void drawCube(GLdouble width, GLdouble height, GLdouble depth) {
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0);
  glScalef(width, height, depth);
  glutWireCube(1.0);
  glPopMatrix();
}

GLdouble colors[2][3] = {
  {0.0, 1.0, 0.0}, // sphere 1 color = green
  {1.0, 0.0, 0.0} // sphere 2 color = red
};

void drawSphere(GLdouble x, GLdouble y, GLdouble z, GLdouble radius, GLdouble rotate, GLdouble * color) {
  glPushMatrix();
  //translates the current matrix to (x, y, z)
  glTranslatef(x, y, z);
  glColor3f(color[0], color[1], color[2]);
  glRotatef(-90, 1.0, 0.0, 0.0); // rotate 90 deg about x to bring (0,0,1) to (0, 1, 0)
  glRotatef(rotate, 0.0, 0.0, 1.0); // rotate around z some degree amount
  // draw the sphere
  glutWireSphere(radius, 20, 20);
  glPopMatrix();
}

void drawGrid() {
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
  for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
    glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
    glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
  }
  glEnd();
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
    case 'i': {
      objX -= delta;
      break;
    }
    case 'j': {
      objZ -= delta;
      break;
    }
    case 'k': {
      objZ += delta;
      break;
    }
    case 'l': {
      objX += delta;
      break;
    }
    default: return;
  }
  glutPostRedisplay();
}

/**
 * Rendering the window.
 */
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw some grid lines
  drawGrid();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // translate object as defined by the
  glTranslatef(objX, objY, objZ);

  drawCube(2.5, 1.0, 1.0);
  drawSphere(-0.7,1.0,0.0, 0.5, sphere1Rotate, colors[0]);
  drawSphere(0.7,1.0,0.0, 0.5, sphere2Rotate, colors[1]);

  glPopMatrix();
  // end object

  glFlush();
  glutSwapBuffers();
}

/**
 * When the window reshapes to a new size
 * @param w the window new width
 * @param h the window new height
 */
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);

  // setting the current matrix mode to GL_PROJECTION
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /** Fovy, aspect, zNear, zFar */
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 400.0);
  // reset the current matrix mode to GL_MODELVIEW to be used in display
  glMatrixMode(GL_MODELVIEW);
}

/**
 * Every time the timer ticks
 */
void renderTick(int value) {
  sphere1Rotate = fmod(sphere1Rotate + rotateDelta1, 360);
  sphere2Rotate = fmod(sphere2Rotate - rotateDelta2, 360);
  glutPostRedisplay();
  glutTimerFunc(timerMs, renderTick, 1); // restart the timer
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Task 2: Hierarchical Object");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  // Look from point (200, 200, 200) at point (100, 10, 100)
  // with the up vector (0, 1, 0)
  gluLookAt(2,2,2, 0,0,0, 0,1,0);
  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
