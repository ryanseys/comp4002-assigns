/**
 * COMP 4002 - Assignment 3
 *
 * Task 1 (40 marks): Create a Gouraud light model.
 * Task 2 (40 marks): Create a Phong Lighting model.
 * Task 3 (10 marks): Compare the lighting models. Which model is better? Why?
 * Task 4 (10 marks): Perform bilinear interpolation.
 *
 * Author: Ryan Seys - 100817604
 */

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#define INIT_GLEW(); // Don't glewInit on Mac
#else
#include <glew.h>
#include <GL\freeglut.h>
#define INIT_GLEW(); glewInit();
#endif

#include <stdio.h>
#include <cmath>
#include "math.h"
#include "Shader.h"
#include "ryan_sphere.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
// #include "ryan_robotarm.h"
#include "ryan_light.h"

GLdouble delta = 0.1; // how much to move the object (Task 2)

GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = 0.0;

GLint robotPartSelected = -1; // nothing initially selected
GLfloat ROBOT_ROTATE_DEG = 1.0;

GLuint gouraudShaderProg, phongShaderProg;
GLuint activeShaderProgram;
GLint windowHeight, windowWidth;

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.2;

Vector3f position (103, 13, 103);
Vector3f lookAtPoint(100, 10, 100);
Vector3f upVector(0, 0, 1);

// initialize camera
Camera * cam;

Light * light;

GLdouble rotateDelta1 = 0.1; // Rotate first sphere 0.1 degrees per frame
GLdouble rotateDelta2 = 0.2; // Rotate second sphere 0.2 degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 20;

// Robot arm
// RobotArm * robotarm;
SolidSphere * sphere0;
SolidSphere * sphere1;
SolidSphere * sphere2;
SolidCube * cube;

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
      // robotarm->rotatePart(robotPartSelected, ROBOT_ROTATE_DEG);
      break;
    }
    case 'x': {
      // Rotate robot part -1 degree
      // robotarm->rotatePart(robotPartSelected, -ROBOT_ROTATE_DEG);
      break;
    }
    case '+': {
      printf("+ pressed\n");
      break;
    }
    case '-': {
      printf("- pressed\n");
      break;
    }
    case 'M': {
      printf("M pressed - turn ambient light off.\n");
      light->setAmbient(0.0, 0.0, 0.0);
      break;
    }
    case 'm': {
      printf("m pressed - turn ambient light on.\n");
      light->setAmbient(1.0, 1.0, 1.0); // default
      break;
    }
    case 'N': {
      printf("N pressed - turn diffuse light off.\n");
      light->setDiffuse(0.0, 0.0, 0.0);
      break;
    }
    case 'n': {
      printf("n pressed - turn diffuse light on.\n");
      light->setDiffuse(1.0, 1.0, 1.0);
      break;
    }
    case 'B': {
      printf("B pressed - turn specular light off.\n");
      light->setSpecular(0.0, 0.0, 0.0);
      break;
    }
    case 'b': {
      printf("b pressed - turn specular light on.\n");
      light->setSpecular(1.0, 1.0, 1.0);
      break;
    }
    case 'c': {
      printf("c pressed - change the colour of the light sources.\n");
      light->setAmbient(0, 1.0, 0.5);
      light->setDiffuse(0.7, 0, 0.7);
      light->setSpecular(1, 0, 0);
      // ambient colour to (0, 1.0, 0.5)
      // diffuse colour to (0.7, 0, 0.7)
      // specular light to (1, 0, 0)
      break;
    }
    case 'C': {
      printf("C pressed - light change back to white.\n");
      light->setAmbient(1.0, 1.0, 1.0);
      light->setDiffuse(1.0, 1.0, 1.0);
      light->setSpecular(1.0, 1.0, 1.0);
      break;
    }
    case 'i': {
      printf("i pressed - increase scale sphere in the x-direction by 0.5 increments.\n");
      break;
    }
    case 'j': {
      printf("j pressed - decrease scale sphere in the x-direction by 0.5 increments.\n");
      break;
    }
    case 'k': {
      printf("k pressed - increase scale sphere in the y-direction by 0.5 increments.\n");
      break;
    }
    case 'l': {
      printf("l pressed - decrease scale sphere in the y-direction by 0.5 increments.\n");
      break;
    }
    case 'r': {
      printf("r pressed - reset the scale of sphere in x,y,z to 1.\n");
      break;
    }
    case 'p': {
      printf("p pressed - Use Phong model.\n");
      activeShaderProgram = phongShaderProg;
      break;
    }
    case 'P': {
      printf("P pressed - Use Phong model.\n");
      activeShaderProgram = phongShaderProg;
      break;
    }
    case 'g': {
      printf("g pressed - Use Gouraud model.\n");
      activeShaderProgram = gouraudShaderProg;
      break;
    }
    case 'G': {
      printf("G pressed - Use Gouraud model.\n");
      activeShaderProgram = gouraudShaderProg;
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
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0, 0.3, 1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  Matrix4f initTranslateMat = Matrix4f::translation(100, 10, 100);

  // setting up the transformaiton of the object from model coord. system to world coord.
  // Matrix4f worldMat =initTranslateMat;

  glUseProgram(activeShaderProgram);

  GLuint viewMatLoc = glGetUniformLocation(activeShaderProgram,  "viewMat");
  glUniformMatrix4fv(viewMatLoc, 1, 1, (float *) cam->getViewMatrix().vm);

  GLuint projMatLoc = glGetUniformLocation(activeShaderProgram,  "projMat");
  glUniformMatrix4fv(projMatLoc, 1, 1, (float *) cam->getProjMatrix().vm);

  GLuint lightAmbLoc = glGetUniformLocation(activeShaderProgram,  "lightAmb");
  glUniform4fv(lightAmbLoc, 1, (float *) &light->ambient);

  GLuint lightDiffLoc = glGetUniformLocation(activeShaderProgram,  "lightDiff");
  glUniform4fv(lightDiffLoc, 1, (float *) &light->diffuse);

  GLuint lightSpecLoc = glGetUniformLocation(activeShaderProgram,  "lightSpec");
  glUniform4fv(lightSpecLoc, 1, (float *) &light->specular);

  GLuint lightPosLoc = glGetUniformLocation(activeShaderProgram,  "lightPos");
  glUniform4fv(lightPosLoc, 1, (float *) &light->position);

  // sphere0->applyTransformation(worldMat);
  // Matrix4f objMat = Matrix4f::translation(objX, objY, objZ);

  // sphere1->applyTransformation(worldMat);
  // sphere1->translate(-0.7, 1.0, -1.25);
  // sphere1->applyTransformation(objMat);
  // sphere1->rotateY(sphere1Rotate);

  // sphere2->applyTransformation(worldMat);
  // sphere2->translate(0.7, 1.0, -1.25);
  // sphere2->applyTransformation(objMat);
  // sphere2->rotateY(sphere2Rotate);

  // cube->applyTransformation(worldMat);
  // cube->applyTransformation(objMat);
  // cube->translate(-0.25, -0.25, -1.6);

  // robotarm->applyTransformation(worldMat);
  // robotarm->applyTransformation(Matrix4f::translation(-3, 0.0, 1.0));

  // draw them spheres, applying all transformations

  sphere0->translate(100, 10, 100);
  sphere0->rotateY(sphere1Rotate += 5);
  sphere0->rotateX(sphere1Rotate);
  sphere0->drawSphere(activeShaderProgram);
  // sphere1->drawSphere(shaderProg);
  // sphere2->drawSphere(shaderProg);
  // cube->draw(shaderProg);
  // robotarm->draw(shaderProg);

  glUseProgram(0);
  glFlush();
  glutSwapBuffers();
}

/**
 * When the window reshapes to a new size, you must update the camera.
 *
 * @param w the window new width
 * @param h the window new height
 */
void reshape(GLint w, GLint h) {
  cam->reshape(w, h);
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
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("COMP 4002 - Assignment 3 - Ryan Seys");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  INIT_GLEW();

  s.createShaderProgram("gouraud.vert", "gouraud.frag", &gouraudShaderProg);
  s.createShaderProgram("phong.vert", "phong.frag", &phongShaderProg);

  activeShaderProgram = gouraudShaderProg; // default gouraud shader

  // For Task 1.
  sphere0 = new SolidSphere(1, 50, 50);

  sphere0->setAmbient(0.8, 0.4, 0.2);
  sphere0->setDiffuse(1, 1, 1);
  sphere0->setSpecular(0.8, 0.8, 0.8);

  // // Object for Task 2.
  // cube = new SolidCube(1.0, 0.5, 0.5);
  // sphere1 = new SolidSphere(0.75, 24, 24);
  // sphere2 = new SolidSphere(0.75, 24, 24);

  // For Task 3.
  cam = new Camera(position, lookAtPoint, upVector);

  light = new Light();

  light->setAmbient(1.0, 1.0, 1.0);
  light->setDiffuse(1.0, 1.0, 1.0);
  light->setSpecular(1.0, 1.0, 1.0);
  light->setPosition(200, 210, 200, 0);

  // Robot arm for Task 4 (Bonus)
  // robotarm = new RobotArm();

  glutPostRedisplay();
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
