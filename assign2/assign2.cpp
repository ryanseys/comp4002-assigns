/**
 * COMP 4002 - Assignment 2
 *
 * Task 1: Render a sphere at (100, 10, 100) using perspective projection.
 * Task 2: Render a hierarchical object beside the sphere from Task 1.
 * Task 3: Create a camera class with yaw, pitch and roll.
 *
 * Author: Ryan Seys - 100817604
 */
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>
#include "ryan_sphere.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_robotarm.h"
#include <stdio.h>

GLdouble initX = 100.0;
GLdouble initY = 10.0;
GLdouble initZ = 100.0;

GLdouble delta = 0.1;
// translate entire scene to (100,10,100)
GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = -1.1;

GLdouble armX = -1.0;
GLdouble armY = 0.0;
GLdouble armZ = 1.1;

GLint robotPartSelected = -1; // nothing initially selected
GLfloat ROBOT_ROTATE_DEG = 1.0;

Camera cam;

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.2;

Vector3f camInitPoint(102.0, 12.0, 102.0);
Vector3f camLookAtPoint(100.0, 10.0, 100.0);
Vector3f camUp(0.0, 1.0, 0.0);

GLdouble rotateDelta1 = 0.1; // degrees per frame
GLdouble rotateDelta2 = 0.2; // degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 20;

SolidSphere sphere0(0.5, 24, 24);
SolidSphere sphere1(0.5, 24, 24);
SolidSphere sphere2(0.5, 24, 24);
SolidCube cube(2.5, 1.0, 1.0);

// Robot arm
RobotArm robotarm;

/**
 * Draw a grid.
 */
void drawGrid() {
  glColor3f(0.5, 0.5, 0.5);
  glBegin(GL_LINES);
  for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
    glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
    glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
  }
  glEnd();
}

/**
 * When a regular (not special) key is pressed.
 */
void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
    case '1': {

      robotPartSelected = 1;
      break;
    }
    case '2': {
      robotPartSelected = 2;
      break;
    }
    case '3': {
      robotPartSelected = 3;
      break;
    }
    case '4': {
      robotPartSelected = 4;
      break;
    }
    case '5': {
      robotPartSelected = 5;
      break;
    }
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
    case 'a': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. -PITCH_AMT vs. PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam.roll(-PITCH_AMT);
      cam.refresh();
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam.roll(PITCH_AMT);
      cam.refresh();
      break;
    }
    case 'w': {
      // Move camera forward along lookAtVector
      cam.moveForward(FORWARD_AMT);
      cam.refresh();
      break;
    }
    case 's': {
      // Move camera backward along lookAtVector
      cam.moveForward(-FORWARD_AMT);
      cam.refresh();
      break;
    }
    case 'z': {
      // Rotate robot part +1 degree
      robotarm.rotatePart(robotPartSelected, ROBOT_ROTATE_DEG);
      break;
    }
    case 'x': {
      // Rotate robot part -1 degree
      robotarm.rotatePart(robotPartSelected, -ROBOT_ROTATE_DEG);
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

  // draw some grid lines and regular sphere from task 1
  glPushMatrix();
  // glMatrixMode(GL_MODELVIEW);
  glTranslatef(initX, initY, initZ);

  drawGrid();

  sphere0.draw(0.0, 0.0, 0.0, 0.0);
  glPopMatrix();

  // object
  // glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(initX + objX, initY + objY, initZ + objZ);

  cube.draw(-0.5, -0.5, -0.5);
  sphere1.draw(-0.7, 1.0, 0.0, sphere1Rotate);
  sphere2.draw(0.7, 1.0, 0.0, sphere2Rotate);
  glPopMatrix();

  // Start Robot arm
  robotarm.draw(initX + armX, initY + armY, initZ + armZ);

  glUseProgram(0);
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
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 1000.0);
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

void pressSpecialKey(int key, int xx, int yy) {
  switch (key) {
    case GLUT_KEY_UP: {
      cam.pitch(PITCH_AMT);
      cam.refresh();
      break;
    }
    case GLUT_KEY_DOWN: {
      cam.pitch(-PITCH_AMT);
      cam.refresh();
      break;
    }
    case GLUT_KEY_RIGHT: {
      cam.yaw(YAW_AMT);
      cam.refresh();
      break;
    }
    case GLUT_KEY_LEFT: {
      cam.yaw(-YAW_AMT);
      cam.refresh();
      break;
    }
  }
  glutPostRedisplay();
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("COMP 4002 - Assignment 2 - Ryan Seys");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  cam.setCamera(camInitPoint, camLookAtPoint, camUp);
  cam.refresh();

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
