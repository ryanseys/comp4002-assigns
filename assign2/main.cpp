
// #include "stdafx.h"
#include <iostream>

using namespace std;

#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants

#include "math.h"
#include "nuss_matrix.h"

// #include <windows.h>


// #include "glew.h"
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif
// #include "gl/gl.h"

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))

GLuint programIDs[2];

struct sphereVertex {
	float pos[4];
	float normal[4];	// the average normal
	short numFaces;		// number of faces shared by the vertex
	long colora;		// ambient colour	- change to a colour structure
    long colord;        // diffuse color    - change to a colour structure
	long colors;		// specular colour  - change to a colour structure

};

GLuint vertex_handle;   // Vertex handle that contains interleaved positions and colors
GLuint triangle_handle; // Triangle handle that contains triangle indices

struct sphereVertex *vtx = NULL;
int numVtx = 0;
GLuint *ind = NULL;
int numInd = 0;
int scalex = 1,scaley=1, scalez=1;

int numTriangles;

int createSphere(int numLong, int numLat, float radius, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1) ;
int createCylinder(int numLong, float radius, float height, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1) ;


/**************************************************************************************/
//  Define the geometry


void Init_Geometry()
{

	// create a shpere wiht 64 longitudes and 64 latitudes
	//createSphere(64,32, 1, &vtx, &numVtx, &ind, &numInd);
	createCylinder(64,100, 15, &vtx, &numVtx, &ind, &numInd);

	return;

}


/*************************************************************************************/
// create VBO objects

void InitVBO()
{
	int rc = 0;
    //Create the vertex handle and copy the data to the GPU memory
    glGenBuffers(1, &vertex_handle);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		printf("error in attach shaders \n");
	}
    glBindBuffer(GL_ARRAY_BUFFER, vertex_handle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct sphereVertex)*numVtx, vtx, GL_STATIC_DRAW);

    //Create the triangle handle, which is an array of indices, and copy the data to the GPU memory
    glGenBuffers(1, &triangle_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numInd, ind, GL_STATIC_DRAW);
}

void display()
{
}

/**************************************************************************************/
// main


int main(int argc, char** argv)
{
	int rc;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(10, 10);

	glutCreateWindow("My Window");
	glutDisplayFunc(display);

	initShaders(&programIDs[0], &vertexShaders[0], &fragmentShaders[0]);

	// initialize GLEW
	// GLenum err = glewInit();
	// if ( err != GLEW_OK)  printf(" Error initializing GLEW! \n");

	Init_Geometry();
	InitVBO();


	glutMainLoop();
}


/**************************************************************************************/
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

int createSphere(int numLong, int numLat, float radius, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1)

{
	int rc = 0;
	int i,j,k;
	float alpha = 0.0;	// angle of latitude starting from the "south pole" at angle -90
	float beta = 0.0;   // angle of longtitude in the rage of 0-360
	float deltaAlpha;
	float deltaBeta;
	int numRows;
	int numCols;

	//int numTriangles;



	numRows = numLat*2;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

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
    deltaBeta = (float)360.0/(numLong);	// increment of beta

	for(i = 0, alpha = -90; i <= numRows; i++ ,alpha += deltaAlpha) {
		for(j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			(*vtx)[k].normal[2] = sin(DegreeToRadians(alpha));  // z coordinate
			(*vtx)[k].normal[0] = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));   // x coordinate
			(*vtx)[k].normal[1] = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta));	// y coordinate
			(*vtx)[k].normal[3] = 0.0;
			struct sphereVertex v;
			v = (*vtx)[k];

			// position in space
			(*vtx)[k].pos[0]  = (*vtx)[k].normal[0] * radius;
			(*vtx)[k].pos[1]  = (*vtx)[k].normal[1] * radius;
			(*vtx)[k].pos[2]  = (*vtx)[k].normal[2] * radius;
			(*vtx)[k].pos[3]  = 1.0;
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
	//float u[4],v[4],w[4];	// used to cmpute the normals - w is the normal
	//float rangeMax = 10;
	//float rangeMin = 0;
	float alpha = 0.0;	// angle of latitude starting from the "south pole" at angle -90
	float beta = 0.0;   // angle of longtitude in the rage of 0-360
	float deltaAlpha;
	float deltaBeta;
	int numRows;
	int numCols;




	numRows = 1;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

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
	deltaAlpha = 90;	// increment of alpha
	beta = 0;   // angle of the longtidute
    deltaBeta = (float) 360.0/(numLong);	// increment of beta

	for(i = 0, alpha = -45; i <= numRows; i++ ,alpha += deltaAlpha) {
		for(j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			(*vtx)[k].normal[2] = sin(DegreeToRadians(alpha));  // z coordinate
			(*vtx)[k].normal[0] = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));   // x coordinate
			(*vtx)[k].normal[1] = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta));	// y coordinate
			(*vtx)[k].normal[3] = 0.0;

			// position in space
			(*vtx)[k].pos[0]  = (*vtx)[k].normal[0] * radius;
			(*vtx)[k].pos[1]  = (*vtx)[k].normal[1] * radius;
			(*vtx)[k].pos[2]  = (*vtx)[k].normal[2] * radius;
			(*vtx)[k].pos[3]  = (float) (alpha < 0) ? -height/2.0 : height/.2;

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
