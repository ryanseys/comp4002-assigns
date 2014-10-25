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
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>

GLuint vbo;
GLuint programID;
GLuint vertexShader;
GLuint fragmentShader;

int logLength;
GLchar * logMsg;
GLint link_status = GL_FALSE;
GLint compile_status = GL_FALSE;

GLdouble delta = 0.1;
GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = 0.0;

GLdouble rotateDelta1 = 0.1; // degrees per frame
GLdouble rotateDelta2 = 0.2; // degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 20;

const int NUM_VERTICES = 8;
const int NUM_FACES = 6;

class SolidCube {
protected:
  GLint vertices[NUM_VERTICES][3] = {
      {0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
      {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};

  GLint faces[NUM_FACES][4] = {
      {1, 5, 7, 3}, {5, 4, 6, 7}, {4, 0, 2, 6},
      {3, 7, 6, 2}, {0, 1, 3, 2}, {0, 4, 5, 1}};

  GLfloat vertexColors[NUM_VERTICES][3] = {
      {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
      {1.0, 0.0, 0.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}};

  GLfloat height;
  GLfloat width;
  GLfloat depth;

public:
  SolidCube(GLfloat w, GLfloat h, GLfloat d) {
    width = w;
    height = h;
    depth = d;
  }

  void draw() {
    glPushMatrix();
    glScalef(width, height, depth);
    glTranslatef(-0.5, -0.5, -0.5);

    glBegin(GL_QUADS);
    for (int i = 0; i < NUM_FACES; i++) {
      for (int j = 0; j < 4; j++) {
        glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
        glVertex3iv((GLint*)&vertices[faces[i][j]]);
      }
    }
    glEnd();
    glPopMatrix();
  }
};

/**
 * This sphere class was borrowed from: http://tinyurl.com/onmhley
 *
 * It was modified by Ryan Seys to add colors to the spheres.
 */
class SolidSphere {
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    std::vector<GLfloat> colors; // added colors

public:
  SolidSphere(float radius, unsigned int rings, unsigned int sectors) {
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);
    int r, s;

    vertices.resize(rings * sectors * 3);
    normals.resize(rings * sectors * 3);
    texcoords.resize(rings * sectors * 2);
    colors.resize(rings * sectors * 3);
    std::vector<GLfloat>::iterator v = vertices.begin();
    std::vector<GLfloat>::iterator n = normals.begin();
    std::vector<GLfloat>::iterator t = texcoords.begin();
    std::vector<GLfloat>::iterator c = colors.begin();

    for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
      float const y = sin( -M_PI_2 + M_PI * r * R );
      float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
      float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

      *t++ = s*S;
      *t++ = r*R;

      *v++ = x * radius;
      *v++ = y * radius;
      *v++ = z * radius;

      *n++ = x;
      *n++ = y;
      *n++ = z;

      // add colors
      *c++ = 76;
      *c++ = 30;
      *c++ = 25;
    }

    indices.resize(rings * sectors * 4);
    std::vector<GLushort>::iterator i = indices.begin();
    for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
      *i++ = r * sectors + s;
      *i++ = r * sectors + (s+1);
      *i++ = (r+1) * sectors + (s+1);
      *i++ = (r+1) * sectors + s;
    }
  }

  void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat rotate) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotate, 0.0, 1.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    glColorPointer(3, GL_UNSIGNED_BYTE, 0, &colors[0]);

    glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
    glPopMatrix();
  }
};

SolidSphere sphere1(0.5, 24, 24);
SolidSphere sphere2(0.5, 24, 24);

SolidCube cube(2.5, 1.0, 1.0);

void checkShaderForErrors(GLuint shaderID) {
  // check shader for errors
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_status);
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

  logMsg = new GLchar[logLength + 1];

  glGetShaderInfoLog(shaderID, logLength, NULL, logMsg);

  if (compile_status != GL_TRUE) {
    fprintf(stderr, "Error in shader(%i): %s\n", shaderID, logMsg);
    std::exit(EXIT_FAILURE);
  }
}

void checkLinkForErrors(GLuint programID) {
  glGetProgramiv(programID, GL_LINK_STATUS, &link_status);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

  logMsg = new GLchar[logLength + 1];

  glGetProgramInfoLog(programID, logLength, NULL, logMsg);

  if (link_status != GL_TRUE) {
    fprintf(stderr, "Error in linking program: %s\n", logMsg);
    std::exit(EXIT_FAILURE);
  }
}

std::string read_file(std::string filename) {
  std::ifstream t(filename);
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

void initShaders() {
  // create shaders
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the shader code from file
  std::string vertSource = read_file("sphere.vert");
  std::string fragSource = read_file("sphere.frag");

  // store as const * char
  const char * vertSourceStr = vertSource.c_str();
  const char * fragSourceStr = fragSource.c_str();

  // attach source code to shaders
  glShaderSource(vertexShader, 1, &vertSourceStr, NULL);
  glShaderSource(fragmentShader, 1, &fragSourceStr, NULL);

  // compile shaders and check for errors
  glCompileShader(vertexShader);
  checkShaderForErrors(vertexShader);

  glCompileShader(fragmentShader);
  checkShaderForErrors(fragmentShader);

  // create program
  programID = glCreateProgram();
  glAttachShader(programID, fragmentShader);
  glAttachShader(programID, vertexShader);

  // link and check for errors
  glLinkProgram(programID);
  checkLinkForErrors(programID);
}

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

  cube.draw();
  // drawCube(2.5, 1.0, 1.0);
  // drawSphere(-0.7,1.0,0.0, 0.5, sphere1Rotate, colors[0]);
  // drawSphere(0.7,1.0,0.0, 0.5, sphere2Rotate, colors[1]);

  sphere1.draw(-0.7, 1.0, 0.0, sphere1Rotate);
  sphere2.draw(0.7, 1.0, 0.0, sphere2Rotate);

  glPopMatrix();
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
  glutTimerFunc(1, renderTick, 1);

  glutMainLoop();

  glDeleteProgram(programID);
  return 0;
}
