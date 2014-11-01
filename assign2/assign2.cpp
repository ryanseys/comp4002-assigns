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

#include <stdio.h>
#include <cmath>
#include "math.h"
#include "Shader.h"
#include "ryan_sphere.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_robotarm.h"

GLdouble initX = 100.0;
GLdouble initY = 10.0;
GLdouble initZ = 100.0;

GLdouble delta = 0.1;
// translate entire scene to (100,10,100)
GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = 0.0;

GLdouble armX = -1.0;
GLdouble armY = 0.0;
GLdouble armZ = 1.1;

GLint robotPartSelected = -1; // nothing initially selected
GLfloat ROBOT_ROTATE_DEG = 1.0;

GLuint shaderProg;
GLuint cubeShaderProg;

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.2;

Vector3f position (103, 13, 103);
Vector3f lookAtPoint(100, 10, 100);
Vector3f upVector(0, 1, 0);

// initialize camera
Camera * cam;

GLdouble rotateDelta1 = 0.1; // degrees per frame
GLdouble rotateDelta2 = 0.2; // degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 20;

// Robot arm
// RobotArm robotarm;
SolidSphere * sphere0;
SolidSphere * sphere1;
SolidSphere * sphere2;
SolidCube * cube;

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
      cam->roll(-PITCH_AMT);
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->roll(PITCH_AMT);
      break;
    }
    case 'w': {
      // Move camera forward along lookAtVector
      cam->moveForward(FORWARD_AMT);
      break;
    }
    case 's': {
      // Move camera backward along lookAtVector
      cam->moveForward(-FORWARD_AMT);
      break;
    }
    case 'z': {
      // Rotate robot part +1 degree
      // robotarm.rotatePart(robotPartSelected, ROBOT_ROTATE_DEG);
      break;
    }
    case 'x': {
      // Rotate robot part -1 degree
      // robotarm.rotatePart(robotPartSelected, -ROBOT_ROTATE_DEG);
      break;
    }
    default: return;
  }
  glutPostRedisplay();
}

GLfloat deg = 0.0;
GLfloat deg2 = 0.0;

/**
 * Rendering the window.
 */
void display() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  // glClearColor(1.0, 0.0, 0, 1);

  Matrix4f initTranslateMat = Matrix4f::translation(100, 10, 100);
  // setting up the transformaiton of the object from model coord. system to world coord.

  Matrix4f m = cam->getViewMatrix() * initTranslateMat;

  glUseProgram(shaderProg);

  sphere0->applyTransformation(m);

  Matrix4f translateObjMat = Matrix4f::translation(objX, objY, objZ);

  sphere1->applyTransformation(m);
  sphere1->translate(-0.7, 1.0, -1.0);
  sphere1->applyTransformation(translateObjMat);
  sphere1->rotateY(sphere1Rotate += 5);

  sphere2->applyTransformation(m);
  sphere2->translate(0.7, 1.0, -1.0);
  sphere2->applyTransformation(translateObjMat);
  sphere2->rotateY(sphere2Rotate -= 5);

  cube->applyTransformation(m);
  cube->applyTransformation(translateObjMat);
  cube->applyTransformation(Matrix4f::scale(1.5, 0.5, 1.0));
  cube->translate(-0.25, -0.25, -1.6);
  // cube->applyTransformation(translateObjMat);


  // draw them spheres, applying all transformations
  sphere0->drawSphere(shaderProg);
  sphere1->drawSphere(shaderProg);
  sphere2->drawSphere(shaderProg);
  cube->draw(shaderProg);

  // sphere2->drawSphere(shaderProg);

  sphere0->clear();
  sphere1->clear();
  sphere2->clear();
  cube->clear();


  // viewMat.m = (float *) viewMat.vm;
  // projMat.m = (float *) projMat.vm;

  // bind the buffers to the shaders

  // draw some grid lines and regular sphere from task 1
  // glPushMatrix();
  // // glMatrixMode(GL_MODELVIEW);
  // glTranslatef(initX, initY, initZ);

  // drawGrid();

  // sphere0.draw(0.0, 0.0, 0.0, 0.0);
  // glPopMatrix();

  // // object
  // // glMatrixMode(GL_MODELVIEW);
  // glPushMatrix();
  // glTranslatef(initX + objX, initY + objY, initZ + objZ);

  // cube.draw(-0.5, -0.5, -0.5);
  // sphere1.draw(-0.7, 1.0, 0.0, sphere1Rotate);
  // sphere2.draw(0.7, 1.0, 0.0, sphere2Rotate);
  // glPopMatrix();

  // // Start Robot arm
  // robotarm.draw(initX + armX, initY + armY, initZ + armZ);

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
  // gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 1000.0);
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
      cam->pitch(PITCH_AMT);
      break;
    }
    case GLUT_KEY_DOWN: {
      cam->pitch(-PITCH_AMT);
      break;
    }
    case GLUT_KEY_RIGHT: {
      cam->yaw(YAW_AMT);
      break;
    }
    case GLUT_KEY_LEFT: {
      cam->yaw(-YAW_AMT);
      break;
    }
  }
  glutPostRedisplay();
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  Shader s;
  // Shader c;
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("COMP 4002 - Assignment 2 - Ryan Seys");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  s.createShaderProgram("sphere.vert", "sphere.frag", &shaderProg);
  // c.createShaderProgram("cube.vert", "cube.frag", &cubeShaderProg);

  cam = new Camera(position, lookAtPoint, upVector);
  sphere0 = new SolidSphere(0.5, 24, 24);
  sphere1 = new SolidSphere(0.5, 24, 24);
  sphere2 = new SolidSphere(0.5, 24, 24);
  cube = new SolidCube(1.0, 0.5, 0.5);

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
