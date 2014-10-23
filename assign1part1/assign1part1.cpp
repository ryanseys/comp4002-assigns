/**
* Assignment 1 - COMP 4002
* Part 1: Task 1 and 2
*
* Single and recursive triangles - no shaders
*
* Ryan Seys - 100817604
*
* Compile on Mac with:
* $ clang assign1.cpp -o assign -framework OpenGL -framework GLUT && ./assign
*/
#include <stdio.h>
#include <stdlib.h>
// #include <GL/glew.h>
#if defined(__APPLE__)
#include <GL/freeglut.h>
#else
#include <glut.h>
#endif


#define NUM_VTX 4 /* number of end points to draw */

// First and second window ids
int singleTriWin;
int recursiveTriWin;

// Window width and height
int widthWin = 500; // window width
int heightWin = 400; // window height

// triangle vertexes
float v1[] = { -1.0, -1.0, 0.0 }; // bottom left: left 1.0, down 1.0
float v2[] = { 1.0, -1.0, 0.0 }; // bottom right: right 1.0, down 1.0
float v3[] = { 0.0, 1.0, 0.0 }; // top center: up 1.0

// triangle colours
float colourWhite[] = { 1.0, 1.0, 1.0 };
float colourRed[] = { 1.0, 0.0, 0.0 };
float colourBlue[] = { 0.0, 0.0, 1.0 };
float colourGreen[] = { 0.0, 1.0, 0.0 };

/**
* Initialize application
*/
void initApp(void)
{
	// Add your code here
}

/**
* Draw a triangle on the screen.
* @param v1     first vertex
* @param v2     second vertex
* @param v3     third vertex
* @param colour colour of the triangle
*/
void drawTri(float v1[], float v2[], float v3[], float colour[]) {
	glColor3f(colour[0], colour[1], colour[2]); // colour
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(v1[0], v1[1], v1[2]);
		glVertex3f(v2[0], v2[1], v2[2]);
		glVertex3f(v3[0], v3[1], v3[2]);
	}
	glEnd();
}

/**
* Draw triangles recursively.
*
* @param v1     first vertex
* @param v2     second vertex
* @param v3     third vertex
* @param n      number of recurses
* @param colour colour of triangle
*/
void recursiveTri(float v1[], float v2[], float v3[], int n, float colour[]) {
	float v1v2[3], v2v3[3], v1v3[3]; // midpoint vertexes
	if (n < 1) {
		printf("Error occurred. n must be positive.\n");
	}
	else if (n == 1) {
		drawTri(v1, v2, v3, colour);
	}
	else {
		// midpoint of v1 and v2
		v1v2[0] = (v1[0] + v2[0]) / 2;
		v1v2[1] = (v1[1] + v2[1]) / 2;
		v1v2[2] = (v1[2] + v2[2]) / 2;

		// midpoint of v2 and v3
		v2v3[0] = (v2[0] + v3[0]) / 2;
		v2v3[1] = (v2[1] + v3[1]) / 2;
		v2v3[2] = (v2[2] + v3[2]) / 2;

		// midpoint of v1 and v3
		v1v3[0] = (v1[0] + v3[0]) / 2;
		v1v3[1] = (v1[1] + v3[1]) / 2;
		v1v3[2] = (v1[2] + v3[2]) / 2;

		// recursively render triangles
		recursiveTri(v1, v1v2, v1v3, n - 1, colourGreen);
		recursiveTri(v1v2, v2, v2v3, n - 1, colourBlue);
		recursiveTri(v1v3, v2v3, v3, n - 1, colourRed);
	}
}

/**
* Callback functions for GLUT.
* Renders the window.
*/
void renderWinSingle(void) {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTri(v1, v2, v3, colourWhite); // draw a single triangle

	glFlush();
}

void renderWinRecursive(void) {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	recursiveTri(v1, v2, v3, 4, colourWhite); // draw recursive triangle

	glFlush();
}

/**
* Called when the corresponsing window is created or resized.
* @param w New window width
* @param h New window height
*/
void resizeWin(int w, int h) {

	// save new screen dimensions
	widthWin = w;
	heightWin = h;

	// use the full view port
	glViewport(0, 0, (GLsizei)widthWin, (GLsizei)heightWin);

	// use 2D  orthographic parallel projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// these have been commented out because they didn't work
	// glOrtho(0, widthWin, 0, heightWin, -1.0, 1.0);
	// gluOrtho2D(0.0, widthWin, 0.0, heightWin);
}

/**
* Main to be run
* @param  argc argc
* @param  argv argv
* @return      return code
*/
int main(int argc, char *argv[])
{
	// General initialization which are private to the application
	// This is not related to OpenGL
	initApp();

	// Initialize GLUT and pass it the command variables
	glutInit(&argc, argv);

	// Set the first window instruct OpenGL where to put the window on screen
	glutInitWindowPosition(100, 100);

	// Instruct OpenGL to use a single display buffer and
	// use 4 values for a colour RGB and alpha
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	// Instruct OpenGL what window size to use
	glutInitWindowSize((int)widthWin, (int)heightWin);

	// Create the first window and obtain a handle to it
	// then create and set up resize and display callbacks
	singleTriWin = glutCreateWindow("Single Triangle");
	glutReshapeFunc(resizeWin);
	glutDisplayFunc(renderWinSingle);

	// Set the window position for the second window
	glutInitWindowPosition(600, 100);

	// Create the second window and obtain a handle to it
	// then create and set up resize and display callbacks.
	// This window will display the recursive triangle
	recursiveTriWin = glutCreateWindow("Recursive Triangle");
	glutReshapeFunc(resizeWin);
	glutDisplayFunc(renderWinRecursive);

	// Get information about OpenGL
	printf("Rendering GPU: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	printf("OpenGL vendor: %s \n", glGetString(GL_VENDOR));

	// Start the main loop
	glutMainLoop();
	return 0;
}
