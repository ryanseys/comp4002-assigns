// /**
// * Assignment 2 - COMP 4002
// *
// * Ryan Seys - 100817604
// *
// * Compile on Mac with the Makefile:
// * $ make
// */
// #include <sstream>
// #include <fstream>
// #include <vector>
// #if defined(__APPLE__)
// #include <GL/freeglut.h>
// #else
// #include <glew.h>
// #include <GL\freeglut.h>
// #endif

// // each window has one of these
// int windows[2];
// GLuint VBOs[2];
// GLuint programIDs[2];
// GLuint vertexShaders[2];
// GLuint fragmentShaders[2];

// // related to error checking / logging
// int logLength;
// GLchar * logMsg;
// GLint link_status = GL_FALSE;
// GLint compile_status = GL_FALSE;
// GLfloat dispX = 0.00; // displacement in X. Updated for every frame.

// // Window width and height
// int widthWin = 500; // window width
// int heightWin = 400; // window height

// struct Vertex {
//   GLfloat pos[3]; // x, y, z
//   GLfloat colour[4]; // r, g, b, a
// };

// std::vector<Vertex> vertices[2];

// struct Vertex v1white = { { -1.0, -1.0, 0.0 }, { 1.0, 1.0, 1.0, 1.0 } };
// struct Vertex v2white = { { 1.0, -1.0, 0.0 }, { 1.0, 1.0, 1.0, 1.0 } };
// struct Vertex v3white = { { 0.0, 1.0, 0.0 }, { 1.0, 1.0, 1.0, 1.0 } };

// struct Vertex v1red = { { -1.0, -1.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 } };
// struct Vertex v2green = { { 1.0, -1.0, 0.0 }, { 0.0, 1.0, 0.0, 1.0 } };
// struct Vertex v3blue = { { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0, 1.0 } };

// void printOpenGLInfo(void) {
//   printf("Rendering GPU: %s\n", glGetString(GL_RENDERER));
//   printf("OpenGL version: %s\n", glGetString(GL_VERSION));
//   printf("OpenGL vendor: %s \n", glGetString(GL_VENDOR));
// }

// void checkShaderForErrors(GLuint shaderID) {
//   // check shader for errors
//   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_status);
//   glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

//   logMsg = new GLchar[logLength + 1];

//   glGetShaderInfoLog(shaderID, logLength, NULL, logMsg);

//   if (compile_status != GL_TRUE) {
//     fprintf(stderr, "Error in shader(%i): %s\n", shaderID, logMsg);
//     std::exit(EXIT_FAILURE);
//   }
// }

// void checkLinkForErrors(GLuint programID) {
//   glGetProgramiv(programID, GL_LINK_STATUS, &link_status);
//   glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

//   logMsg = new GLchar[logLength + 1];

//   glGetProgramInfoLog(programID, logLength, NULL, logMsg);

//   if (link_status != GL_TRUE) {
//     fprintf(stderr, "Error in linking program: %s\n", logMsg);
//     std::exit(EXIT_FAILURE);
//   }
// }

// std::string read_file(std::string filename) {
//   std::ifstream t(filename);
//   std::stringstream buffer;
//   buffer << t.rdbuf();
//   return buffer.str();
// }

// void setupDraw(GLuint programID, GLuint vbo, std::vector<Vertex> vertices) {
//   // clear display
//   glClearColor(0.0, 0.0, 0.0, 0.0);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//   glBufferData(
//     GL_ARRAY_BUFFER,
//     sizeof(struct Vertex) * vertices.size(),
//     vertices.data(),
//     GL_STATIC_DRAW
//     );

//   // use shaders
//   glUseProgram(programID);
//   glBindBuffer(GL_ARRAY_BUFFER, vbo);

//   glUniform1f(glGetUniformLocation(programID, "dispX"), dispX);

//   // enable attribute arrays
//   glEnableVertexAttribArray(0);
//   glEnableVertexAttribArray(1);
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void *)offsetof(struct Vertex, pos));
//   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (void*)offsetof(struct Vertex, colour));
// }

// void cleanupDraw(void) {
//   glBindVertexArray(0);
//   glDisableVertexAttribArray(0);
//   glDisableVertexAttribArray(1);
//   glUseProgram(0);
//   glutSwapBuffers();
//   glFlush();
//   glutPostRedisplay();
// }

// void drawTri(struct Vertex v1, struct Vertex v2, struct Vertex v3) {
//   vertices[0].push_back(v1);
//   vertices[0].push_back(v2);
//   vertices[0].push_back(v3);
// }

// void drawTriRecursive(struct Vertex v1, struct Vertex v2, struct Vertex v3, int n) {
//   struct Vertex v1v2, v2v3, v1v3;

//   if (n < 1) {
//     printf("An error occurred. n cannot be < 1.");
//     std::exit(EXIT_FAILURE);
//   }
//   else if (n == 1) {
//     vertices[1].push_back(v1);
//     vertices[1].push_back(v2);
//     vertices[1].push_back(v3);
//   }
//   else {
//     v1v2.pos[0] = (v1.pos[0] + v2.pos[0]) / 2;
//     v1v2.pos[1] = (v1.pos[1] + v2.pos[1]) / 2;
//     v1v2.pos[2] = (v1.pos[2] + v2.pos[2]) / 2;
//     v1v2.colour[0] = v1.colour[0];
//     v1v2.colour[1] = v1.colour[1];
//     v1v2.colour[2] = v1.colour[2];
//     v1v2.colour[3] = v1.colour[3];

//     // midpoint of v2 and v3
//     v2v3.pos[0] = (v2.pos[0] + v3.pos[0]) / 2;
//     v2v3.pos[1] = (v2.pos[1] + v3.pos[1]) / 2;
//     v2v3.pos[2] = (v2.pos[2] + v3.pos[2]) / 2;
//     v2v3.colour[0] = v2.colour[0];
//     v2v3.colour[1] = v2.colour[1];
//     v2v3.colour[2] = v2.colour[2];
//     v2v3.colour[3] = v2.colour[3];

//     // midpoint of v1 and v3
//     v1v3.pos[0] = (v1.pos[0] + v3.pos[0]) / 2;
//     v1v3.pos[1] = (v1.pos[1] + v3.pos[1]) / 2;
//     v1v3.pos[2] = (v1.pos[2] + v3.pos[2]) / 2;
//     v1v3.colour[0] = v3.colour[0];
//     v1v3.colour[1] = v3.colour[1];
//     v1v3.colour[2] = v3.colour[2];
//     v1v3.colour[3] = v3.colour[3];

//     drawTriRecursive(v1, v1v2, v1v3, n - 1);
//     drawTriRecursive(v1v2, v2, v2v3, n - 1);
//     drawTriRecursive(v1v3, v2v3, v3, n - 1);
//   }
// }

// void drawWindow(int winNum) {
//   setupDraw(programIDs[winNum], VBOs[winNum], vertices[winNum]);
//   glDrawArrays(GL_TRIANGLES, 0, vertices[winNum].size());
//   cleanupDraw();
//   vertices[winNum].clear();
// }

// void renderWin(void) {
//   drawTri(v1white, v2white, v3white);
//   drawWindow(0);
// }

// void renderWinRecursive(void) {
//   drawTriRecursive(v1red, v2green, v3blue, 4);
//   drawWindow(1);
// }

// /**
// * Called when the corresponsing window is created or resized.
// * @param w New window width
// * @param h New window height
// */
// void resizeWin(int w, int h) {
//   // save new screen dimensions
//   widthWin = w;
//   heightWin = h;

//   // use the full view port
//   glViewport(0, 0, (GLsizei)widthWin, (GLsizei)heightWin);

//   // use 2D orthographic parallel projection
//   glMatrixMode(GL_PROJECTION);
//   glLoadIdentity();
// }

// void initShaders(GLuint * programID, GLuint * vertexShader, GLuint * fragmentShader) {
//   // create shaders
//   *vertexShader = glCreateShader(GL_VERTEX_SHADER);
//   *fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

//   // Read the shader code from file
//   std::string vertSource = read_file("triangle.vert");
//   std::string fragSource = read_file("triangle.frag");

//   // store as const * char
//   const char * vertSourceStr = vertSource.c_str();
//   const char * fragSourceStr = fragSource.c_str();

//   // attach source code to shaders
//   glShaderSource(*vertexShader, 1, &vertSourceStr, NULL);
//   glShaderSource(*fragmentShader, 1, &fragSourceStr, NULL);

//   // compile shaders and check for errors
//   glCompileShader(*vertexShader);
//   checkShaderForErrors(*vertexShader);

//   glCompileShader(*fragmentShader);
//   checkShaderForErrors(*fragmentShader);

//   // create program
//   *programID = glCreateProgram();
//   glAttachShader(*programID, *fragmentShader);
//   glAttachShader(*programID, *vertexShader);

//   // link and check for errors
//   glLinkProgram(*programID);
//   checkLinkForErrors(*programID);
// }

// void renderTick(int winNum) {
//   // printf("%d %d %d %d\n", viewport[0], viewport[1], viewport[2], viewport[3]);
//   dispX += 0.05;
//   if (dispX > 2) {
//     dispX = -2.0;
//   }
//   glutTimerFunc(20, renderTick, 1);
// }

// // void mouseMoved(int x, int y) {
// //     dispX = x/500.0;
// // }

// /**
// * Main to be run
// * @param  argc argc
// * @param  argv argv
// * @return      return code
// */
// int main(int argc, char *argv[]) {

//   // Initialize GLUT and pass it the command variables
//   glutInit(&argc, argv);

//   // Set the first window instruct OpenGL where to put the window on screen
//   glutInitWindowPosition(100, 100);

//   // Instruct OpenGL to use a single display buffer and
//   // use 4 values for a colour RGB and alpha
//   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

//   // Instruct OpenGL what window size to use
//   glutInitWindowSize((int)widthWin, (int)heightWin);

//   // Create the first window and obtain a handle to it
//   // then create and set up resize and display callbacks
//   windows[0] = glutCreateWindow("Single Triangle Shader");
//   glutReshapeFunc(resizeWin);
//   glutDisplayFunc(renderWin);

//   // glewInit(); // Un-comment to work on windows

//   initShaders(&programIDs[0], &vertexShaders[0], &fragmentShaders[0]);

//   glGenBuffers(1, &VBOs[0]); // Generate 1 buffer
//   glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

//   // Get information about OpenGL
//   printOpenGLInfo();

//   glutTimerFunc(20, renderTick, 1);

//   // Start the main loop
//   glutMainLoop();

//   // clean up
//   glDeleteProgram(programIDs[0]);
//   // glDeleteProgram(programIDs[1]);

//   return 0;
// }


// This program does a little solar system simulation from the point of view
// of a comet.  Here there is a yellow sun and a blue  planet; the planet
// spins on its own axis and it rotates around the sun.  The viewer is
// sitting on a comet and is always facing the sun.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cmath>

void myWireSphere(GLfloat radius, int slices, int stacks) {
  glPushMatrix();
  glTranslatef(100, 10, 100); //translates the current matrix 0 in x, 0 in y and 1 in z

  // In the GLUT library someone put the polar regions on the z-axis - yech!!
  // We fixed it so that they are on the y-axis.  We do this by rotating -90
  // degrees about the x-axis which brings (0,0,1) to (0,1,0).
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  glutWireSphere(radius, slices, stacks);

  glPopMatrix();
}

// As usual, the routine to display the current state of the system is
// bracketed with a clearing of the window and a glFlush call.  Immediately
// within those calls the drawing code itself is bracketed by pushing and
// popping the current transformation.  And also as usual, we are assuming the
// current matrix mode is GL_MODELVIEW.  We finish with a SwapBuffers call
// because we'll animate.
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  // draw the sphere
  glColor3f(1.0, 0.0, 1.0);
  myWireSphere(100.0, 50, 50);
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

// As usual we reset the projection transformation whenever the window is
// reshaped.  This is done (of course) by setting the current matrix mode
// to GL_PROJECTION and then setting the matrix.  It is easiest to use the
// perspective-projection-making matrix from the GL utiltiy library.  Here
// we set a perspective camera with a 60-degree vertical field of view,
// an aspect ratio to perfectly map into the system window, a near clipping
// plane distance of 1.0 and a far clipping distance of 40.0.  The last
// thing done is to reset the current matrix mode to GL_MODELVIEW, as
// that is expected in all the calls to display().
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /** Fovy, aspect, zNear, zFar */
  gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 400.0);
  glMatrixMode(GL_MODELVIEW);
}

// The usual main() for a GLUT application.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Ballin'");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  // Look from point (200, 200, 200) at point (100, 10, 100) with
  // the up vector (0, 1, 0)
  gluLookAt(200,200,200, 100,10,100, 0,1,0);
  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
}
