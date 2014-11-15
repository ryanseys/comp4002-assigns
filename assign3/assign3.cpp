/**
 * COMP 4002 - Assignment 3
 *
 * The following is completed:
 *
 * Task 1 (40 marks): Create a Gouraud light model. [DONE]
 * Task 2 (40 marks): Create a Phong Lighting model. [DONE]
 * Task 3 (10 marks): Compare the lighting models. Which model is better? Why? [DONE - See PDF]
 * Task 4 (10 marks): Perform bilinear interpolation. [DONE - See PDF]
 * Task 5 (25 marks): Bonus: Create a spotlight. [DONE]
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
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_light.h"

GLuint gouraudShaderProg, phongShaderProg;
GLuint activeShaderProgram;
GLint windowHeight, windowWidth;

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 1.0;

// Camera defaults
Vector3f position (103, 13, 103);
Vector3f lookAtPoint(100, 10, 100);
Vector3f upVector(0, 1, 0);

// initialize camera
Camera * cam;
Light * light;
Light * spotlight;

GLfloat shininess = 5.0; // min is zero
GLfloat SHINY_FACTOR = 5.0;

SolidSphere * sphere;

GLfloat sphereScaleX = 1.0;
GLfloat sphereScaleY = 1.0;
GLfloat sphereScaleZ = 1.0;

float addShininess(GLfloat amount) {
  GLfloat SHINY_MIN = 0;
  GLfloat SHINY_MAX = 250;
  shininess += amount;
  if(shininess < SHINY_MIN) {
    shininess = SHINY_MIN;
  } else if(shininess > SHINY_MAX) {
    shininess = SHINY_MAX;
  }
  return shininess;
}

/**
 * When a regular (not special) key is pressed.
 */
void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
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
    case '+': {
      addShininess(SHINY_FACTOR);
      printf("+ pressed - increase shininess to %f.\n", shininess);
      break;
    }
    case '-': {
      addShininess(-SHINY_FACTOR);
      printf("- pressed - decrease shininess to %f.\n", shininess);
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
      light->setAmbient(0.0, 1.0, 0.5);
      light->setDiffuse(0.7, 0.0, 0.7);
      light->setSpecular(1.0, 0.0, 0.0);
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
      sphereScaleX += 0.5;
      break;
    }
    case 'j': {
      printf("j pressed - decrease scale sphere in the x-direction by 0.5 increments.\n");
      sphereScaleX -= 0.5;
      if(sphereScaleX < 1.0) {
        sphereScaleX = 1.0;
      }
      break;
    }
    case 'k': {
      printf("k pressed - increase scale sphere in the y-direction by 0.5 increments.\n");
      sphereScaleY += 0.5;
      break;
    }
    case 'l': {
      printf("l pressed - decrease scale sphere in the y-direction by 0.5 increments.\n");
      sphereScaleY -= 0.5;
      if(sphereScaleY < 1.0) {
        sphereScaleY = 1.0;
      }
      break;
    }
    case 'r': {
      printf("r pressed - reset the scale of sphere in x,y,z to 1.\n");
      sphereScaleX = 1.0;
      sphereScaleY = 1.0;
      sphereScaleZ = 1.0;
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
    case '0': {
      spotlight->coneAngle += 1;
      if(spotlight->coneAngle > 120) {
        spotlight->coneAngle = 120;
      }
      printf("0 pressed - Increase the spotlight cutoff angle to %f deg.\n", spotlight->coneAngle);
      break;
    }
    case '9': {
      spotlight->coneAngle -= 1;
      if(spotlight->coneAngle < 1) {
        spotlight->coneAngle = 1;
      }
      printf("9 pressed - Decrease the spotlight cutoff angle to %f deg.\n", spotlight->coneAngle);
      break;
    }
    case '8': {
      spotlight->angularAtten += 1;
      printf("8 pressed - Increase spotlight angular attenuation to %f deg.\n", spotlight->angularAtten);
      break;
    }
    case '7': {
      spotlight->angularAtten -= 1;
      if(spotlight->angularAtten < 1) {
        spotlight->angularAtten = 1;
      }
      printf("7 pressed - Decrease the spotlight angular attenuation to %f deg.\n", spotlight->angularAtten);
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

  // New origin at (100, 10, 100)
  Matrix4f initTranslateMat = Matrix4f::translation(100, 10, 100);

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

  GLuint shininessLoc = glGetUniformLocation(activeShaderProgram,  "shininess");
  glUniform1f(shininessLoc, shininess);

  GLuint spotPosLoc = glGetUniformLocation(activeShaderProgram,  "spotPos");
  glUniform4fv(spotPosLoc, 1, (float *) &spotlight->position);

  GLuint spotLookAtLoc = glGetUniformLocation(activeShaderProgram,  "spotLookAtPnt");
  glUniform4fv(spotLookAtLoc, 1, (float *) &spotlight->lookAtPoint);

  GLuint spotAngAttenLoc = glGetUniformLocation(activeShaderProgram,  "spotAngAtten");
  glUniform1f(spotAngAttenLoc, spotlight->angularAtten);

  GLuint spotConeAngleLoc = glGetUniformLocation(activeShaderProgram,  "spotConeAngle");
  glUniform1f(spotConeAngleLoc, spotlight->coneAngle);

  sphere->translate(100, 10, 100);
  sphere->scale(sphereScaleX, sphereScaleY, sphereScaleZ);
  sphere->drawSphere(activeShaderProgram);

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

  // Default: Gouraud shader
  activeShaderProgram = gouraudShaderProg;

  // Set up sphere
  sphere = new SolidSphere(1, 25, 25);
  sphere->setAmbient(0.8, 0.4, 0.2); // material ambient color
  sphere->setDiffuse(0.75, 0.75, 0.5); // material diffuse color
  sphere->setSpecular(0.8, 0.8, 0.8); // material specular color

  // Set up camera
  cam = new Camera(position, lookAtPoint, upVector);

  // Set up light
  light = new Light();
  // white light
  light->setAmbient(1.0, 1.0, 1.0);
  light->setDiffuse(1.0, 1.0, 1.0);
  light->setSpecular(1.0, 1.0, 1.0);
  light->setPosition(200, 210, 200);

  // Task 5: Create a spotlight
  spotlight = new Light();
  // white spotlight by default
  spotlight->setAmbient(1.0, 1.0, 1.0);
  spotlight->setDiffuse(1.0, 1.0, 1.0);
  spotlight->setSpecular(1.0, 1.0, 1.0);
  spotlight->setPosition(190, 200, 190);
  spotlight->setLookAtPoint(100, 10, 100); // center of sphere

  glutPostRedisplay();
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
  return 0;
}
