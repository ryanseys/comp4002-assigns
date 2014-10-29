//=============================================================================
// camera.h
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose:
//--------------
// a. demonstarte the usage of a camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a camera
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================

#pragma once
#include "nuss_matrix.h"

class camera
{
public:
	camera(void);
	~camera(void);
	int roll(float angleDeg);
	int pitch(float angleDeg);
	int yaw(float angleDeg);
	Vector3f getPosition(void);
	Vector3f getLookAtPoint(void);
	Vector3f getUpVector(void);
	int changePoitionDelta(float dx, float dy, float dz);	// change position by relative amount
	int changePositionDelta(Vector3f *dv);				// change position by relative amount
	int changeAbsPoition(float x, float y, float z);		// change to a new position in space
	int changeAbsPosition(Vector3f *v);					// change to a new position in space
	Vector3f moveForward(float numUnits);  // moves the camera forward by the numUnits units along the lookAtVector
	void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
	int updateSpeed(float speed);
	float getSpeed(void);



private:
	int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space



public:

	// change the speed of the camera motion
};
