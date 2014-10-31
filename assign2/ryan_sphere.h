#ifndef RYAN_SPHERE
#define RYAN_SPHERE
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "ryan_vector.h"
#include "ryan_matrix.h"

/**
 * This sphere class was derived from: http://tinyurl.com/onmhley
 *
 * It was modified by Ryan Seys to add colors to the spheres and
 * rotation transformation.
 */

int numTriangles;

struct sphereVertex {
  float pos[4];
  float normal[4];  // the average normal
  short numFaces;   // number of faces shared by the vertex
  long colora;    // ambient colour - change to a colour structure
  long colord;        // diffuse color    - change to a colour structure
  long colors;    // specular colour  - change to a colour structure
};

class SolidSphere {
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
    std::vector<GLfloat> colors; // added colors
    GLfloat radius;
    struct sphereVertex * vtx = NULL;
    GLuint sphereVBO;   // Vertex handle that contains interleaved positions and colors
    GLuint triangleVBO; // Triangle handle that contains triangle indices
    GLuint * ind = NULL;
    int numInd;
    std::vector<Matrix4f> transformations;
public:
  // SolidSphere(float radius, unsigned int rings, unsigned int sectors) {
  //   radius = radius;
  //   float const R = 1./(float)(rings-1);
  //   float const S = 1./(float)(sectors-1);
  //   int r, s;
  //   int col = 0;

  //   vertices.resize(rings * sectors * 3);
  //   normals.resize(rings * sectors * 3);
  //   texcoords.resize(rings * sectors * 2);
  //   colors.resize(rings * sectors * 3);
  //   std::vector<GLfloat>::iterator v = vertices.begin();
  //   std::vector<GLfloat>::iterator n = normals.begin();
  //   std::vector<GLfloat>::iterator t = texcoords.begin();
  //   std::vector<GLfloat>::iterator c = colors.begin();

  //   for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
  //     float const y = sin( -M_PI_2 + M_PI * r * R );
  //     float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
  //     float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

  //     *t++ = s*S;
  //     *t++ = r*R;

  //     *v++ = x * radius;
  //     *v++ = y * radius;
  //     *v++ = z * radius;

  //     *n++ = x;
  //     *n++ = y;
  //     *n++ = z;

  //     // add epic colors!
  //     *c++ = x;
  //     *c++ = y;
  //     *c++ = z;
  //   }

  //   indices.resize(rings * sectors * 4);
  //   std::vector<GLushort>::iterator i = indices.begin();
  //   for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
  //     *i++ = r * sectors + s;
  //     *i++ = r * sectors + (s+1);
  //     *i++ = (r+1) * sectors + (s+1);
  //     *i++ = (r+1) * sectors + s;
  //   }
  // }

  SolidSphere(GLfloat radius, GLuint numLong, GLuint numLat) {
    int rc = 0;
    int i,j,k;
    float alpha = 0.0;  // angle of latitude starting from the "south pole" at angle -90
    float beta = 0.0;   // angle of longtitude in the rage of 0-360
    float deltaAlpha;
    float deltaBeta;
    int numRows;
    int numCols;
    numInd = 0;

    numRows = numLat*2;  // number of horizonal slabs
    numCols = numLong;  // number of vertical slabs

    int numVtx = (numRows+1) * (numCols+1);   // define only the north hemisphere

    int numQuads = numRows * numCols;
    numTriangles = numQuads * 2;

    // allocate memory
    vtx = (struct sphereVertex *) malloc(sizeof(struct sphereVertex) * numVtx);
    if (vtx == NULL) {
      // error
      rc = 1;
      printf("Oops! An error occurred\n");
    }

    ind = (GLuint *) malloc(sizeof(GLuint) * numTriangles * 3);
    if (ind == NULL) {
      // error
      rc = 1;
      printf("Oops! An error occurred\n");
    }

    // Fill the vertex buffer with positions
    k = 0;
    alpha = 0.0;  // angle of latitude starting from the "south pole"
    deltaAlpha = (float)90.0 / numLat; // increment of alpha
    beta = 0;   // angle of the longtidute
    deltaBeta = (float)360.0/(numLong); // increment of beta

    for(i = 0, alpha = -90; i <= numRows; i++ ,alpha += deltaAlpha) {
      for(j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
        (vtx)[k].normal[2] = sin(DegreeToRadians(alpha));  // z coordinate
        (vtx)[k].normal[0] = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));   // x coordinate
        (vtx)[k].normal[1] = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta)); // y coordinate
        (vtx)[k].normal[3] = 0.0;
        struct sphereVertex v;
        v = (vtx)[k];

        // position in space
        (vtx)[k].pos[0]  = (vtx)[k].normal[0] * radius;
        (vtx)[k].pos[1]  = (vtx)[k].normal[1] * radius;
        (vtx)[k].pos[2]  = (vtx)[k].normal[2] * radius;
        (vtx)[k].pos[3]  = 1.0;
        k++;
      }
    }

    // fill the index buffer

    k = 0;
    for(i = 0; i < numRows; i++) {
      for(j = 0; j < numCols; j++)
      {
        // fill indices for the quad
        // change by making a quad function
        (ind)[k] = i * (numCols+1) + j;
        (ind)[k+1] = i * (numCols+1) + j + 1;
        (ind)[k+2] = (i+1) * (numCols+1) + j + 1;

        k +=3;
        (ind)[k] = i * (numCols+1) + j;
        (ind)[k+1] = (i+1) * (numCols+1) + j + 1;
        (ind)[k+2] = (i+1) * (numCols+1) + j;

        k+=3;
      }
    }

    // *numVtx1 = numVtx;
    numInd = numTriangles*3;

    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct sphereVertex)*numVtx, vtx, GL_STATIC_DRAW);
    glGenBuffers(1, &triangleVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numTriangles*3, ind, GL_STATIC_DRAW);
  }

  void rotateY(GLfloat degrees) {
    Matrix4f rotateYMat = Matrix4f::rotateY(degrees, true);
    this->applyTransformation(rotateYMat);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    Matrix4f translateZMat = Matrix4f::translation(x, y, z);
    this->applyTransformation(translateZMat);
  }

  /**
   * Adjust the pitch of the sphere by some amount
   * @param degrees Degrees to adjust pitch.
   */
  void pitch(Vector3f position, Vector3f lookAt, GLfloat degrees) {
    // glTranslatef(position.x, position.y, position.z);
    // glRotatef(degrees, lookAt.x, lookAt.y, lookAt.z);
    // glTranslatef(-position.x, -position.y, -position.z);
  }

  void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat rotate) {
    // glMatrixMode(GL_MODELVIEW);
    // glPushMatrix();
    // glTranslatef(x, y, z);
    // glRotatef(rotate, 0.0, 1.0, 0.0);

    // glEnableClientState(GL_VERTEX_ARRAY);
    // glEnableClientState(GL_NORMAL_ARRAY);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // glEnableClientState(GL_COLOR_ARRAY);

    // glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    // glNormalPointer(GL_FLOAT, 0, &normals[0]);
    // glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    // glColorPointer(3, GL_FLOAT, 0, &colors[0]);

    // glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
    // glPopMatrix();
  }

  void drawSphere(GLuint shaderProg) {
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
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleVBO);

    // Tells OpenGL how to walk through the two VBOs
    struct sphereVertex v;
    int relAddress = (char *) v.pos - (char *) &v;
    glVertexAttribPointer(positionLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);
    relAddress = (char *) v.normal - (char *) &v;
    glVertexAttribPointer(normalLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);

    // draw the triangles
    glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (char*) NULL+0);
  }

  void applyTransformation(Matrix4f m) {
    this->transformations.push_back(m);
  }

  void clear() {
    this->transformations.clear();
  }
};

#endif
