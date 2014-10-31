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
GLdouble objZ = -1.1;

GLdouble armX = -1.0;
GLdouble armY = 0.0;
GLdouble armZ = 1.1;

GLint robotPartSelected = -1; // nothing initially selected
GLfloat ROBOT_ROTATE_DEG = 1.0;

GLuint shaderProg;

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

// SolidSphere sphere0(0.5, 24, 24);
// SolidSphere sphere1(0.5, 24, 24);
// SolidSphere sphere2(0.5, 24, 24);
// SolidCube cube(2.5, 1.0, 1.0);

// Robot arm
// RobotArm robotarm;
SolidSphere * sphere;
SolidSphere * sphereTwo;

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
      cam->refresh();
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->roll(PITCH_AMT);
      cam->refresh();
      break;
    }
    case 'w': {
      // Move camera forward along lookAtVector
      cam->moveForward(FORWARD_AMT);
      cam->refresh();
      break;
    }
    case 's': {
      // Move camera backward along lookAtVector
      cam->moveForward(-FORWARD_AMT);
      cam->refresh();
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
  glClearColor(0.0,0.0,0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0,0.0,0,1);

  // Matrix4f viewMat, projMat, modelMat;

  // the next three lines of code mimic a camera.
  // the lines should be replaced with camera position

  Matrix4f initTranslateMat = Matrix4f::translation(100, 10, 100);

  // Matrix4f rotateMat = Matrix4f::rotateY(deg += 5.0, true);
  // Matrix4f rotateMat2 = Matrix4f::rotateY(deg2 -= 5.0, true);
  // Matrix4f translateMat = Matrix4f::scale(1.5, 0.0, 0);
  Matrix4f translateMat = Matrix4f::translation(2.0, 0.0, 0.0);
  // setting up the transformaiton of the object from model coord. system to world coord.

  Matrix4f m = cam->getViewMatrix() * initTranslateMat;

  glUseProgram(shaderProg);

  sphere->drawSphere(shaderProg, m);
  sphereTwo->drawSphere(shaderProg, m * translateMat);

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
      cam->pitch(PITCH_AMT);
      cam->refresh();
      break;
    }
    case GLUT_KEY_DOWN: {
      cam->pitch(-PITCH_AMT);
      cam->refresh();
      break;
    }
    case GLUT_KEY_RIGHT: {
      cam->yaw(YAW_AMT);
      cam->refresh();
      break;
    }
    case GLUT_KEY_LEFT: {
      cam->yaw(-YAW_AMT);
      cam->refresh();
      break;
    }
  }
  glutPostRedisplay();
}


/**************************************************************************************/
/* this function creates a cylinder.  The cylinder is genreated using two arrays.  The vertex data array which
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles
are stored in the index array as a triangle list.

Input
numLong - number of longitudes lines.  For example if numLong == 10 than the sphere is divided into 10
of 36 degrees each


Radius - the radius of the sphere.  Note, that this is superfluous since it can be achieved by scaling the
sphere.

height - the height of the cylinder - Note that this is superfluous since it can be achieved by scaling.

Ouptut:
vtx - a buffer with all the vertex information.  Currently the function computes the position information
and the normal of each vertex.  Note, the array is allocted by the function.

numVtx1 - returns the number of vertices there were genrated.

ind - a buffer which contains the topology of the triangles.

numInd 1 - the number of entries in the buffer.

Return:
the function returns 0 is successful.


*/
int createCylinder(int numLong, float radius, float height, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1)

{
  int rc = 0;
  int i,j,k;
  //float rowPos[4] = {0.0,1.0,0.0,1.0};
  //float colPos[4] = {0.0,1.0,0.0,1.0};
  //float u[4],v[4],w[4]; // used to cmpute the normals - w is the normal
  //float rangeMax = 10;
  //float rangeMin = 0;
  float alpha = 0.0;  // angle of latitude starting from the "south pole" at angle -90
  float beta = 0.0;   // angle of longtitude in the rage of 0-360
  float deltaAlpha;
  float deltaBeta;
  int numRows;
  int numCols;

  numRows = 1;  // number of horizonal slabs
  numCols = numLong;  // number of vertical slabs

  int numVtx = (numRows+1) * (numCols+1);   // define only the north hemisphere

  int numQuads = numRows * numCols;
  numTriangles = numQuads * 2 + 2*(numCols-2);   // number of triangles on the wall + number of triangles top and bottom

  // allocate memory
  *vtx = (struct sphereVertex *) malloc(sizeof(struct sphereVertex) * numVtx);
  if (vtx == NULL) {
    // error
    rc = 1;
    goto err;
  }

  *ind = (GLuint *) malloc(sizeof(GLuint) * numTriangles * 3);
  if (ind == NULL) {
    // error
    rc = 1;
    goto err;
  }

  // Fill the vertex buffer with positions
  k = 0;
  alpha = 0.0;  // angle of latitude starting from the "south pole"
  deltaAlpha = 90;  // increment of alpha
  beta = 0;   // angle of the longtidute
    deltaBeta = 360.0/(numLong);  // increment of beta

  for(i = 0, alpha = -45; i <= numRows; i++ ,alpha += deltaAlpha) {
    for(j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
      (*vtx)[k].normal[2] = sin(DegreeToRadians(alpha));  // z coordinate
      (*vtx)[k].normal[0] = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));   // x coordinate
      (*vtx)[k].normal[1] = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta)); // y coordinate
      (*vtx)[k].normal[3] = 0.0;

      // position in space
      (*vtx)[k].pos[0]  = (*vtx)[k].normal[0] * radius;
      (*vtx)[k].pos[1]  = (*vtx)[k].normal[1] * radius;
      (*vtx)[k].pos[2]  = (*vtx)[k].normal[2] * radius;
      (*vtx)[k].pos[3]  = (alpha < 0) ? -height/2.0 : height/.2;

      k++;
    }
  }



  // fill the index buffer of the walls

  k = 0;
  for(i = 0; i < numRows; i++) {
    for(j = 0; j < numCols; j++)
    {
      // fill indices for the quad
      (*ind)[k] = i * (numCols+1) + j;
      (*ind)[k+1] = i * (numCols+1) + j + 1;
      (*ind)[k+2] = (i+1) * (numCols+1) + j + 1;

      k +=3;
      (*ind)[k] = i * (numCols+1) + j;
      (*ind)[k+1] = (i+1) * (numCols+1) + j + 1;
      (*ind)[k+2] = (i+1) * (numCols+1) + j;

      k+=3;
    }
  }


  // fill the index buffer of the top plate
  for (i = 0, j = numCols+2; i < numCols - 2; i++, j++) {
      // fill indices of the top plate
      (*ind)[k] = numCols+1;
      (*ind)[k+1] = j;
      (*ind)[k+2] = j+1;
      k +=3;
  }


  // fill the index buffer of the bottom plate
  for (i = 0, j = 1; i < numCols - 2; i++, j++) {
      // fill indices of the top plate
      (*ind)[k] = 0;
      (*ind)[k+1] = j+1;
      (*ind)[k+2] = j;
      k +=3;
  }

  *numVtx1 = numVtx;
  *numInd1 = numTriangles*3;


  return(0);
err:
  return(rc);
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  Shader s;
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("COMP 4002 - Assignment 2 - Ryan Seys");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  s.createShaderProgram("sphere.vert", "sphere.frag", &shaderProg);

  cam = new Camera(position, lookAtPoint, upVector);
  sphere = new SolidSphere(1.0, 16, 16);
  sphereTwo = new SolidSphere(1.0, 16, 16);

  cam->refresh();

  // glutPostRedisplay();
  // glEnable(GL_DEPTH_TEST);
  // glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
