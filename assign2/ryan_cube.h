#ifndef RYAN_CUBE
#define RYAN_CUBE

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

  /**
   * Rotate cube around the y-axis
   * @param degrees Number of degrees to rotate
   */
  void rotate(GLfloat degrees) {
    glTranslatef(width/2, 0.0, depth/2); // half of the cube width and height
    glRotatef(degrees, 0.0, 1.0, 0.0);
    glTranslatef(-(width/2), 0.0, -(depth/2)); // inverse transformation
  }

  void draw(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix();
    glScalef(width, height, depth);
    glTranslatef(x, y, z);

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

#endif
