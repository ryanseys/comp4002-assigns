#pragma once

//=============================================================================
// shader.h
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose:
//--------------
// a. demonstarte the usage of a skybox
//
//
// Description:
//--------------
// A simple class for loading OpenGL shaders
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================



#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include "GL/glut.h"
#include <windows.h>
#include "glew.h"
#endif

class Shader
{
public:
	Shader(void);
	~Shader(void);
	char * readCode(char * fileName);
	int createShaderObj(char* fileName , int shaderType, GLuint *shaderid);

	// functions creates a shader program.  The two shader programs (vertex and fragment) were already compiled.
	GLint ceateShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint *shaderProgId);

private:
	GLuint vertShaderid;
	GLuint fragShaderid;
public:
	GLuint shaderProgramid;
	// creates a shader program from files vsFileName and fsFileName
	int createShaderProgram(char * vsFileName, char * fsFileName, GLuint *shaderProgramid);
	int shaderStatus(void);
};

