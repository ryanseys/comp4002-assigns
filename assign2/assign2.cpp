#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  //translates the current matrix to (100,10,100)
  glTranslatef(100, 10, 100);
  glColor3f(1.0, 0.0, 1.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  // draw the sphere
  glutWireSphere(100.0, 50, 50);

  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

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

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Ballin'");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  // Look from point (200, 200, 200) at point (100, 10, 100) with the up vector (0, 1, 0)
  gluLookAt(200,200,200, 100,10,100, 0,1,0);
  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return 0;
}
