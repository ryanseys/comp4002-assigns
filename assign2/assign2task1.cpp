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
#include "ryan_sphere.h"

SolidSphere sphere(100.0, 24, 24);

/**
 * Rendering the window.
 */
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  sphere.draw(100, 10, 100, 0);

  glPopMatrix();
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
  gluPerspective(55.0, (GLfloat)w/(GLfloat)h, 1.0, 400.0);
  // reset the current matrix mode to GL_MODELVIEW to be used in display
  glMatrixMode(GL_MODELVIEW);
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Task 1: Sphere");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  // Look from point (200, 200, 200) at point (100, 10, 100)
  // with the up vector (0, 1, 0)
  gluLookAt(200,200,200, 100,10,100, 0,1,0);
  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return 0;
}
