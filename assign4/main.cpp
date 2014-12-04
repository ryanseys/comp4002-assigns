//=============================================================================
// main.cpp
//
// Author: Doron Nussbaum (C) 2014 All Rights Reserved.
// Updated by Ryan Seys (100817604) for the purposes of Assignment 4 - COMP 4002
//-----------------------------------------------------
//
// Purpose:
//--------------
// a. demonstarte the usage of a skybox
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a skybox
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by
// the author.
//
// Disclaimer
//--------------
// The code is provided as is without any warranty

// INCLUDE Section
#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants

#include "math.h"

#include "nuss_matrix.h"
#include "shader.h"
#include "camera.h"
#include "SOIL.h"
#include "SkyBox.h"
#include "call_back_functions.h"

// DATA STRUCTURES SECTION
struct sphereVertex {
  float pos[4];
  float normal[4];     // the average normal
  float texCoord1[2];
  float texCoord2[2];
  short numFaces;      // number of faces shared by the vertex not used
  long colora;         // ambient colour  - change to a colour structure
  long colord;         // diffuse color    - change to a colour structure
  long colors;         // specular colour  - change to a colour structure
};

// GLOBALS SECTION

GLuint vertex_handle;   // Vertex handle that contains interleaved positions and colors
GLuint triangle_handle;   // Triangle handle that contains triangle indices
GLuint vertex_handle2;    // Vertex handle that contains interleaved positions and colors
GLuint triangle_handle2;    // Triangle handle that contains triangle indices
GLuint tex;         // a general texture

// the cube data
// nuss remove char *cubeImageFileName="cubeFile";
GLuint skyboxProg;      // shader program for the skybox. Here it is created by main().
              // however it should be internal to skybox class

GLuint shaderProg;      // general shader program used for the surface
GLuint sphereBoxProg;   // shader program for the sphere
GLuint sphereLinesBoxProg;  // shader program for the sphere to draw lines.  It can be
              // set by a flag but was created for debuggin purposes.

SkyBox skybox;        // the skybox
SkyBox skybox2;       // the second skybox

// structures for creating an object: verteices, indices, etc.  (should be moved to an object class)
struct sphereVertex *vtx = NULL;    // array of vertices for a sphere, surface, etc.
int numVtx = 0;
GLuint *ind = NULL;
int numInd = 0;
int numTriangles;  // number of triangles

struct sphereVertex *vtx2 = NULL;   // array of vertices for a sphere, surface, etc.
int numVtx2 = 0;
GLuint *ind2 = NULL;
int numInd2 = 0;
int numTriangles2;  // number of triangles

int scalex = 1,scaley=1, scalez=1;    // used to manipulate the model (should be moved to a class)
float angle =0;

camera cam;               // a camera
int t= 0;               // time

int refractFlag = 0;    // a flag for rendering with reflection vector or refracting vectors

// FUNCTION DECLARATIONS
int createSphere(int numLong, int numLat, float radius, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1) ;
// int createCylinder(int numLong, float radius, float height, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1) ;
// void createQuad(GLuint *ind, int bottomLeft, int bottomRight, int topLeft, int topRight);
// int createSurface(int numSurfaceRows, int numSurfaceCols, float height, float width, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1);
int checkError();

//  Define the geometry
void Init_Geometry(){

  // create a shpere wiht 64 longitudes and 64 latitudes
  // createSphere(8,4, 1, &vtx, &numVtx, &ind, &numInd);
  createSphere(64,32, 1, &vtx, &numVtx, &ind, &numInd);

  createSphere(64,32, 1, &vtx2, &numVtx2, &ind2, &numInd2);
  //createCylinder(64,100, 15, &vtx, &numVtx, &ind, &numInd);
  //createSurface(50,50, 20, 20, &vtx, &numVtx, &ind, &numInd);
  return;

}

//  loading a texture
void loadTexture(GLuint *tex, GLuint target, char *imageFileName) {
  int width, height;
  unsigned char* image;

  // generate the texture
  glGenTextures(1, tex);

  image = SOIL_load_image(imageFileName, &width, &height, 0, SOIL_LOAD_RGB);
  glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);


  glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

//  loading a texture
void loadTextures() {

  char *sbTextureNameSunnyDay[6] ={
    "TropicalSunnyDayLeft2048.png",
    "TropicalSunnyDayRight2048.png",
    "TropicalSunnyDayUp2048.png",
    "TropicalSunnyDayDown2048.png",
    "TropicalSunnyDayFront2048.png",
    "TropicalSunnyDayBack2048.png"
  };

  char *sbTextureNight[6] ={
    "right.jpg",
    "left.jpg",
    "top.jpg",
    "bottom.jpg",
    "front.jpg",
    "back.jpg"
  };

  char *texFileName = "sample.png";
  //  NOT USED IN THIS EXAMPLE.   LEFT HERE TO SHOW HOW TO LOAD A TEXTURE
  // loadTexture(&tex, GL_TEXTURE_2D, texFileName);

  // in case one would like to load a colour skybox:
  // front (posZ) is purple (magenta),
  // back (negZ) is yellow,
  // left (negX) is green
  // right (posX) is red
  // top (posY) is blue)
  // bottom (negY) is cyan
  //skybox.loadColourTexture();
  skybox.loadSkybox(sbTextureNameSunnyDay);
  skybox2.loadSkybox(sbTextureNight);
}

// create VBO objects
void InitVBO()
{
  int rc = 0;
  //Create the vertex handle and copy the data to the GPU memory
  glGenBuffers(1, &vertex_handle);
  glGenBuffers(1, &vertex_handle2);

  rc = glGetError();
  if (rc != GL_NO_ERROR) {
    printf("error in attach shaders \n");
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct sphereVertex)*numVtx, vtx, GL_STATIC_DRAW);

  // second sphere
  glBindBuffer(GL_ARRAY_BUFFER, vertex_handle2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct sphereVertex)*numVtx2, vtx2, GL_STATIC_DRAW);

  //Create the triangle handle, which is an array of indices, and copy the data to the GPU memory
  glGenBuffers(1, &triangle_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numInd, ind, GL_STATIC_DRAW);

  // second sphere
  glGenBuffers(1, &triangle_handle2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle2);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numInd2, ind2, GL_STATIC_DRAW);
}

// LOAD THE SHADERS
// NOTE SHADERS CAN BE TRANSFERED TO THE OBJECT UNLESS THEY ARE SHARED

// initialize the shader
int loadShaders(Shader s) {
  int rc = 0;

  // rc = s.createShaderProgram("sphere.vert","sphere.frag", &shaderProg);
  // if (rc != 0) {
  //   printf(" error after generating the spere shader \n");
  //   rc = 1;
  // }

  // initialize the sphere line shader
  // rc = s.createShaderProgram("sphereLinesBox.vert","sphereLinesBox.frag", &sphereLinesBoxProg);
  // if (rc != 0) {
  //   printf(" error after generating sphere line shader \n");
  //   rc = 2;
  // }

  rc = s.createShaderProgram("sphereBox.vert","sphereBox.frag", &sphereBoxProg);
  if (rc != 0) {
    printf(" error after generating sphere skybox shaders \n");
    rc = 4;
  }

  return(rc);
}

// // rendering function to draw an object using a texture
// // note that clearing the frame and depth buffers is done outside this function
// void displayFun() {
//  Matrix4f viewMat, projMat, modelMat, m;

//  // set up the mode to wireframe
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  //set up the mode to fill
//  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//  // setting up the transformaiton of the object from model coord. system to world coord.
//  modelMat = Matrix4f::identity();
//  //modelMat = Matrix4f::translation(0,0,0);
//  modelMat = Matrix4f::scale(7,7,7);


//  // setting up the viewpoint transformation
//  viewMat = cam.getViewMatrix(NULL);

//  // setting up the projection transformation
//  //projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(30, 1, .1, 500);
//  projMat= cam.getProjectionMatrix(NULL);

//  // putting it all together
//  m = projMat * viewMat * modelMat;

//  // load the program to the shader
//  glUseProgram(shaderProg);

//  // transfer the matrix to the shader
//  GLuint locMat= 0;
//  locMat=glGetUniformLocation(shaderProg,  "modelViewProjMat");
//  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

//  // NOTE THAT ONE MAY WANT TO TRANSFER THE OTHER MATRICES (E.G., modelView MATRIX)
//  // AND THE INVERSE MATRIX

//  // set the time
//  GLuint tLoc = glGetUniformLocation(shaderProg, "t");
//  glUniform1i(tLoc, t);

//    // bind the buffers to the shaders
//  GLuint positionLoc = glGetAttribLocation(shaderProg, "vertex_position");
//  GLuint normalLoc = glGetAttribLocation(shaderProg, "vertex_normal");
//  GLuint texLoc = glGetAttribLocation(shaderProg, "texCoord");

//  glEnableVertexAttribArray(positionLoc);
//  glEnableVertexAttribArray(normalLoc);
//  glEnableVertexAttribArray(texLoc);
//  glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
//  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle);

//  // Tell OpenGL how to walk through the two VBOs
//  struct sphereVertex v;
//  int relAddress = (char *) v.pos - (char *) &v;
//  glVertexAttribPointer(positionLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);
//  relAddress = (char *) v.normal - (char *) &v;
//  glVertexAttribPointer(normalLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);
//  relAddress = (char *) v.texCoord1 - (char *) &v;
//  glVertexAttribPointer(texLoc,2,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);

//  // draw the triangles
//  glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (char*) NULL+0);
//  glUseProgram(0);

//  // NOTE SWAP BUFFERS IS CARRIED OUT OUTSIDE THIS FUNCION
//  return;
// }

// RENDERS AN OBJECT USING A SKYBOX AS THE TEXTURE SOURCE
void displayBoxFun(GLuint shaderProg) {
  Matrix4f viewMat, projMat, modelMat, m;
  // set up the mode to wireframe

  // setting up the transformaiton of the object from model coord. system to world coord.
  modelMat = Matrix4f::identity();
  //modelMat = Matrix4f::translation(0,0,0);

  modelMat = Matrix4f::scale(50,50,50);

  // ROATE THE OBJECT AROUND THE CAMERA VECTOR
  // CAN ADD ROTATIONS AROUND THE PRIMARY AXIS
  modelMat = modelMat * Matrix4f::rotateVector(cam.getLookAtVector(),angle,1);

  // setting up the viewpoint transformation
  viewMat = cam.getViewMatrix(NULL);

  // setting up the projection transformation
  projMat= cam.getProjectionMatrix(NULL);

  // putting it all together
  m = projMat * viewMat * modelMat;

  // tell openfl which shader program to use
  glUseProgram(shaderProg);

  // transfer the modelViewProjection  matrix to the shader
  GLuint locMat= 0;
  locMat=glGetUniformLocation(shaderProg,  "modelViewProjMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  // transfer the modelView matrix to the shader
  m = viewMat * modelMat;
  locMat=glGetUniformLocation(shaderProg,  "modelViewMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  // transfer the model matrix to the shader
  m = modelMat;
  locMat=glGetUniformLocation(shaderProg,  "modelMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  // load the camera position to the shader
  locMat=glGetUniformLocation(shaderProg,  "camPos");
  Vector3f camPos = cam.getPosition();
  glUniform3fv(locMat,1, (float *) &camPos);

  // load the refract flag to the shader
  locMat=glGetUniformLocation(shaderProg,  "refractFlag");
  glUniform1i(locMat, refractFlag);

  glActiveTexture(GL_TEXTURE3);
  GLuint texCube = skybox.getTexHandle();
  glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
  GLuint samLoc = glGetUniformLocation(shaderProg, "texCube");
  glUniform1i(samLoc, 3);

  GLint ttt = 0;
  glGetUniformiv(shaderProg, samLoc, &ttt);

  // bind the buffers to the shaders
  GLuint positionLoc = glGetAttribLocation(shaderProg, "vertex_position");
  GLuint normalLoc = glGetAttribLocation(shaderProg, "vertex_normal");

  glEnableVertexAttribArray(positionLoc);
  glEnableVertexAttribArray(normalLoc);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle);

  // Tells OpenGL how to walk through the two VBOs
  struct sphereVertex v;
  int relAddress = (char *) v.pos - (char *) &v;
  glVertexAttribPointer(positionLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);
  relAddress = (char *) v.normal - (char *) &v;
  glVertexAttribPointer(normalLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);

  // draw the triangles
  glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (char*) NULL+0);

  // glUseProgram(0);
  // glUseProgram(shaderProg);

  // draw the second sphere

  modelMat = Matrix4f::identity();
  //modelMat = Matrix4f::translation(0,0,0);

  modelMat = Matrix4f::scale(50,50,50) * Matrix4f::translation(2, 0, 0);

  // ROATE THE OBJECT AROUND THE CAMERA VECTOR
  // CAN ADD ROTATIONS AROUND THE PRIMARY AXIS
  modelMat = modelMat * Matrix4f::rotateVector(cam.getLookAtVector(),angle,1);

  // setting up the viewpoint transformation
  viewMat = cam.getViewMatrix(NULL);

  // setting up the projection transformation
  projMat= cam.getProjectionMatrix(NULL);

  // putting it all together
  m = projMat * viewMat * modelMat;

  locMat= 0;
  locMat=glGetUniformLocation(shaderProg,  "modelViewProjMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  // transfer the modelView matrix to the shader
  m = viewMat * modelMat;
  locMat=glGetUniformLocation(shaderProg,  "modelViewMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  // transfer the model matrix to the shader
  m = modelMat;
  locMat=glGetUniformLocation(shaderProg,  "modelMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  glActiveTexture(GL_TEXTURE3);
  GLuint texCube2 = skybox2.getTexHandle();
  glBindTexture(GL_TEXTURE_CUBE_MAP, texCube2);
  GLuint samLoc2 = glGetUniformLocation(shaderProg, "texCube");
  glUniform1i(samLoc2, 3);

  GLint ttt2 = 0;
  glGetUniformiv(shaderProg, samLoc, &ttt2);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_handle2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle2);

  // Tells OpenGL how to walk through the two VBOs
  // struct sphereVertex v;
  relAddress = (char *) v.pos - (char *) &v;
  glVertexAttribPointer(positionLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);
  relAddress = (char *) v.normal - (char *) &v;
  glVertexAttribPointer(normalLoc,4,GL_FLOAT, GL_FALSE, sizeof(struct sphereVertex),(char*) NULL+relAddress);

  // draw the triangles
  glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (char*) NULL+0);

  return;
}

// this is the render function.  It contains flags to set up the differnt shaders and what
// to draw.  One can remove most of is not all of theem
void renderFun() {
  // DEBUGGING FLAGS
  static int drawTri = 1;
  static int drawSkybox = 1;
  static int drawLines = 0;
  static Vector3f pos=Vector3f(0,0,100);
  static Vector3f lookat=Vector3f(0 ,0 ,0);
  static Vector3f up=Vector3f(0,1,0);
  static int zoom = -70;
  static int camZoom = 1;
  static int setCam = 0;

  // setting up the camera manually if needed
  // can be remved
  if (setCam) {
    cam.setCamera(pos, lookat, up);
    setCam = 0;
  }

  // setting up the camera zoom manually if needed
  // can be remved
  if (camZoom) {
    cam.zoomIn(zoom);
    camZoom = 0;
  }

  // setting opengl - clearing the buffers
  glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects
  glClearColor(1.0,1.0,1.0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  if (drawSkybox) {
    skybox.displaySkybox(cam);
  }

  if (drawTri) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    displayBoxFun(sphereBoxProg);
  }

  // if (drawLines) {
  //   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //   displayBoxFun(sphereLinesBoxProg);
  // }

  glutSwapBuffers();
}

void idleFun() {
  t++;
  // Sleep(10);
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  int rc;
  Shader s;

  // set the camera to an initial position
  cam.setCamera(Vector3f(0,-10,100), Vector3f(0,0,0), Vector3f(0,1,0));

  glutInit(&argc, argv);
  // glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH|GLUT_3_2_CORE_PROFILE);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(10, 10);

  glutCreateWindow("skybox");
  glutDisplayFunc(renderFun);
  glutIdleFunc(idleFun);
  // setCallbackFun();

  // initialize GLEW
  // GLenum err = glewInit();
  // if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n");

  loadTextures();
  skybox.init();
  // skybox2.init();

  Init_Geometry();
  InitVBO();

  rc = loadShaders(s);
  if (rc != 0) {
    printf("error after createing shaders rc = %d \n", rc);
    getchar();
  }

  glutMainLoop();
}

/* this function creates a sphere.  The sphere is genreated using two arrays.  The vertex data array which
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles
are stored in the index array as a triangle list.

Input
numLong - number of longitudes lines.  For example if numLong == 10 than the sphere is divided into 10
of 36 degrees each
numLat - number latitudes in the northern hemispher.  The function calculates the required numbe for the
souhthern hemisphere.  For example if numLat = 5 then there will be 5 sections in the northern hemishpere
and 5 sections in the southern hemisphere.
Radius - the radius of the sphere.  Note, that this is superfluous since it can be achieved by scaling the
sphere.

Ouptut:
vtx - a buffer with all the vertex information.  Currently the function computes the position information
and the normal of each vertex.  Note, the array is allocted by the function.
numVtx1 - returns the number of vertices there were genrated.
ind - a buffer which contains the topology of the triangles.
numInd 1 - the number of entries in the buffer.

Return:
the function returns 0 is successful.
*/
int createSphere(int numLong, int numLat, float radius, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1) {
  int rc = 0;
  int i,j,k;
  float alpha = 0.0;  // angle of latitude starting from the "south pole" at angle -90
  float beta = 0.0;   // angle of longtitude in the rage of 0-360
  float deltaAlpha;
  float deltaBeta;
  float deltaxTex;
  float deltayTex;
  int numRows;
  int numCols;

  //int numTriangles;
  numRows = numLat*2;  // number of horizonal slabs
  numCols = numLong;  // number of vertical slabs

  int numVtx = (numRows+1) * (numCols+1);   // define only the north hemisphere

  int numQuads = numRows * numCols;
  numTriangles = numQuads * 2;

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
  deltaAlpha = (float)90.0 / numLat; // increment of alpha
  beta = 0;   // angle of the longtidute
    deltaBeta = (float)360.0/(numLong); // increment of beta
  deltaxTex = (float) 1.0/numCols;
  deltayTex = (float) 1.0/numRows;


  for(i = 0, alpha = -90; i <= numRows; i++ ,alpha += deltaAlpha) {
    for(j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
      (*vtx)[k].normal[2] = sin(DegreeToRadians(alpha));  // z coordinate
      (*vtx)[k].normal[0] = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));   // x coordinate
      (*vtx)[k].normal[1] = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta)); // y coordinate
      (*vtx)[k].normal[3] = 0.0;

      // position in space
      (*vtx)[k].pos[0]  = (*vtx)[k].normal[0] * radius;
      (*vtx)[k].pos[1]  = (*vtx)[k].normal[1] * radius;
      (*vtx)[k].pos[2]  = (*vtx)[k].normal[2] * radius;
      (*vtx)[k].pos[3]  = 1.0;
      (*vtx)[k].texCoord1[0] = j * deltaxTex;
      (*vtx)[k].texCoord1[1] = i * deltaxTex;
      struct sphereVertex v;
      v = (*vtx)[k];

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

  *numVtx1 = numVtx;
  *numInd1 = numTriangles*3;

  return(0);
err:
  return(rc);
}

int checkError() {
  int rc = glGetError();
  switch (rc) {
  case GL_NO_ERROR:
    rc = 0;
    break;
  case GL_INVALID_ENUM:
    printf("gl error - not a legal enum \n");
    rc = 1;
    break;
  case GL_INVALID_VALUE:
    printf("gl error - not a legal value \n");
    rc = 2;
    break;

  case GL_INVALID_OPERATION:
    printf("gl error - not a legal operation \n");
    rc = 3;
    break;


  case GL_INVALID_FRAMEBUFFER_OPERATION:
    printf("gl error - stack underflow \n");
    rc = 5;
    break;

  case GL_OUT_OF_MEMORY:
    printf(" gl error - out of memory \n");
    rc = 6;
    break;


  default:
    printf("gl error - default \n");
    rc = 9;
    break;
  }
  return(rc);
}
