#ifndef RYAN_CUBE
#define RYAN_CUBE
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "ryan_vector.h"
#include "ryan_matrix.h"

const int NUM_VERTICES = 8;
const int NUM_FACES = 6;

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, 1.0, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f, 1.0,
    -1.0f, 1.0f, 1.0f, 1.0,// triangle 1 : end
    1.0f, 1.0f,-1.0f,  1.0,// triangle 2 : begin
    -1.0f,-1.0f,-1.0f, 1.0,
    -1.0f, 1.0f,-1.0f, 1.0,// triangle 2 : end
    1.0f,-1.0f, 1.0f, 1.0,
    -1.0f,-1.0f,-1.0f, 1.0,
    1.0f,-1.0f,-1.0f, 1.0,
    1.0f, 1.0f,-1.0f, 1.0,
    1.0f,-1.0f,-1.0f, 1.0,
    -1.0f,-1.0f,-1.0f, 1.0,
    -1.0f,-1.0f,-1.0f, 1.0,
    -1.0f, 1.0f, 1.0f, 1.0,
    -1.0f, 1.0f,-1.0f, 1.0,
    1.0f,-1.0f, 1.0f, 1.0,
    -1.0f,-1.0f, 1.0f, 1.0,
    -1.0f,-1.0f,-1.0f, 1.0,
    -1.0f, 1.0f, 1.0f, 1.0,
    -1.0f,-1.0f, 1.0f, 1.0,
    1.0f,-1.0f, 1.0f, 1.0,
    1.0f, 1.0f, 1.0f, 1.0,
    1.0f,-1.0f,-1.0f, 1.0,
    1.0f, 1.0f,-1.0f, 1.0,
    1.0f,-1.0f,-1.0f, 1.0,
    1.0f, 1.0f, 1.0f, 1.0,
    1.0f,-1.0f, 1.0f, 1.0,
    1.0f, 1.0f, 1.0f, 1.0,
    1.0f, 1.0f,-1.0f, 1.0,
    -1.0f, 1.0f,-1.0f, 1.0,
    1.0f, 1.0f, 1.0f, 1.0,
    -1.0f, 1.0f,-1.0f, 1.0,
    -1.0f, 1.0f, 1.0f, 1.0,
    1.0f, 1.0f, 1.0f, 1.0,
    -1.0f, 1.0f, 1.0f, 1.0,
    1.0f,-1.0f, 1.0f, 1.0
};

static const GLfloat g_color_buffer_data[] = {
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    1.0,  0.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  0.0,  1.0,
    1.0,  0.0,  0.0,
    0.0,  1.0,  0.0,
    0.0,  0.0,  1.0
};

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
  std::vector<Matrix4f> transformations;
  GLuint vertexbuffer;
  GLuint colorBuffer;
public:
  SolidCube(GLfloat w, GLfloat h, GLfloat d) {
    width = w;
    height = h;
    depth = d;

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &colorBuffer);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
  }

  /**
   * Rotate cube around the y-axis
   * @param degrees Number of degrees to rotate
   */
  void rotate(GLfloat degrees) {
    Matrix4f translateDown = Matrix4f::translation(width/2, 0.0, depth/2);
    this->applyTransformation(translateDown);
    Matrix4f rotateYMat = Matrix4f::rotateY(degrees, true);
    this->applyTransformation(rotateYMat);
    Matrix4f translateUp = Matrix4f::translation(-(width/2), 0.0, -(depth/2));
    this->applyTransformation(translateUp);
    // glTranslatef(width/2, 0.0, depth/2); // half of the cube width and height
    // glRotatef(degrees, 0.0, 1.0, 0.0);
    // glTranslatef(-(width/2), 0.0, -(depth/2)); // inverse transformation
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    Matrix4f translateZMat = Matrix4f::translation(x, y, z);
    this->applyTransformation(translateZMat);
  }

  void draw(GLfloat x, GLfloat y, GLfloat z) {
    // glPushMatrix();
    // glScalef(width, height, depth);
    // glTranslatef(x, y, z);

    // glBegin(GL_QUADS);
    // for (int i = 0; i < NUM_FACES; i++) {
    //   for (int j = 0; j < 4; j++) {
    //     glColor3fv((GLfloat*)&vertexColors[faces[i][j]]);
    //     glVertex3iv((GLint*)&vertices[faces[i][j]]);
    //   }
    // }
    // glEnd();
    // glPopMatrix();
  }

  void draw(GLuint shaderProg) {
    Matrix4f matrix = Matrix4f::identity();
    int size = this->transformations.size();
    for(int i = 0; i < size; i++) {
      matrix = matrix * this->transformations.at(i);
    }

    GLuint locMat = 0;
    locMat = glGetUniformLocation(shaderProg,  "modelViewProjMat");
    glUniformMatrix4fv(locMat, 1, 1, (float *) matrix.vm);

    GLuint positionLoc = glGetAttribLocation(shaderProg, "vertex_position");
    GLuint normalLoc = glGetAttribLocation(shaderProg, "vertex_normal");
    glEnableVertexAttribArray(positionLoc);
    glEnableVertexAttribArray(normalLoc);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(positionLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(
      1,                        // attribute. No particular reason for 1, but must match the layout in the shader.
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
    );

    // draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    this->clear();
  }

  void applyTransformation(Matrix4f m) {
    this->transformations.push_back(m);
  }

  void clear() {
    this->transformations.clear();
  }
};

#endif
