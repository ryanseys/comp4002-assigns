
//=============================================================================
// camera.cpp
//
//Author: Doron Nussbaum (C) 2011 All Rights Reserved.
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


#include "camera.h"


/******************************************************************/
/*
Purpose: camera constructor


Descripton:

Return:


*/

camera::camera(void): position(50.0,150.0,50.0), lookAtVector(100.0,0.0,100.0), upVector(0.0,1.0,0.0)
, speed(0), nearPlane(0.1), farPlane(1000), fieldOfView(60), aspectRatio(1.0)
{

}

/******************************************************************/
/*
Purpose: camera destructor


Descripton:

Return:


*/
camera::~camera(void)
{
}


/******************************************************************/
/*
Purpose: change the orientation of the camera (roll transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/


int camera::roll(float angleDeg)
{
	// float angleRad = DegreeToRadians(angleDeg);
	// Vector3f rotVector(0.0,0.0,0.0);


	// // get rotation axis
	// rotVector = lookAtVector;

	// updateOrientation(rotVector, angleRad);


	return 0;
}


/******************************************************************/
/*
Purpose: change the orientation of the camera (pitch transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/

int camera::pitch(float angleDeg)
{
	// float angleRad = DegreeToRadians(angleDeg);
	// Vector4f rotVector(0.0,0.0,0.0,1.0);

	// // get rotation axis
	// rotVector = Vector4f(Vector4f::cross(Vector4f(lookAtVector,1.0), Vector4f(upVector, 1.0)),1.0);

	// updateOrientation(rotVector, angleRad);

	return 0;
}

/******************************************************************/
/*
Purpose: change the orientation of the camera (yaw transformation)


Descripton:

Return:
1 - if failed
0 - if successful

*/



int camera::yaw(float angleDeg)
{
	// float angleRad = DegreeToRadians(angleDeg);
	// Vector4f rotVector(0.0,0.0,0.0,1.0);

	// // get rotation axis
	// rotVector = upVector;

	// updateOrientation(rotVector, angleRad);


	return 0;
}

/******************************************************************/
/*
Purpose: obtains the current position of the camera


Descripton:

Return:
Vector3f - camera position

*/

Vector4f camera::getPosition(void)
{
	Vector4f v = position;
	return (v);
}


/******************************************************************/
/*
Purpose: obtains the lookAt point of the camera


Descripton:

Return:
Vector3f - camera lookAt point

*/


Vector4f camera::getLookAtPoint(void)
{
	return (position + lookAtVector);
}

/******************************************************************/
/*
Purpose: obtains the camera's up vector


Descripton:

Return:
Vector3f - camera upVector

*/



Vector4f camera::getUpVector(void)
{
	return (upVector);
}


/******************************************************************/
/*
Purpose: changes the camera's position relative to its current position


Descripton:

Return:
0 - success

*/


int camera::changePoitionDelta(float dx, float dy, float dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;

	return 0;
}


/******************************************************************/
/*
Purpose: changes the camera's position relative to its current position


Descripton:

Return:
0 - success

*/


int camera::changePositionDelta(Vector3f *dv)
{
	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton:

Return:
0 - success

*/


int camera::changeAbsPoition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton:

Return:
0 - success

*/

int camera::changeAbsPosition(Vector3f *v)
{
	position = Vector4f(*v, 1.0);
	return 0;
}

int camera::changeAbsPosition(Vector3f v)
{
	position = Vector4f(v, 1.0);
	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position along the LootAt vector


Descripton: move the camera forward by the numUnits along the looAtVector

Return:
the new position

*/


Vector3f camera::moveForward(float numUnits)
{
	position += lookAtVector*numUnits;

	return (Vector3f(position.x, position.y, position.z));
}

/******************************************************************/
/*
Purpose:


Descripton:

Return:


*/


int camera::updateOrientation(Vector4f rotVector, float angleRad)
{

	Vector4f xaxis(0.0,0.0,0.0, 1.0);

	// create rotation matrix
	rotMat = Matrix4f::rotateVector(rotVector, angleRad, 0);
	// rotate the camera (up vector and/or looAtVector)
	upVector = rotMat*upVector;
	lookAtVector = rotMat*lookAtVector;

	// update the upVector
	xaxis = Vector4f(Vector4f::cross(upVector, lookAtVector),1.0);
	upVector = Vector4f(Vector4f::cross(lookAtVector, xaxis),1.0);
	upVector.normalize();
	lookAtVector.normalize();
	return 0;
}

/******************************************************************/
/*
Purpose: obtains the view transformation matrix


Descripton:

Return:
the transformation matrix

*/



Matrix4f camera::getViewMatrix(Matrix4f *viewMatrix)
{
	Vector4f lookAtPoint;
	Matrix4f m;

	lookAtPoint = position+lookAtVector;
	m = Matrix4f::cameraMatrix(position, lookAtPoint, upVector);
	if (viewMatrix != NULL) *viewMatrix = m;

	return(m);
}

/******************************************************************/
/*
Purpose: obtains the projection transformation matrix


Descripton:

Return:
the transformation matrix

*/



Matrix4f camera::getProjectionMatrix(Matrix4f *projMatrix)
{
	Matrix4f m;

	m = Matrix4f::symmetricPerspectiveProjectionMatrix(fieldOfView, aspectRatio, nearPlane, farPlane);
	if (projMatrix != NULL) *projMatrix = m;

	return(m);
}


/******************************************************************/
/*
Purpose: set the camera parameters


Descripton:

Return:


*/


void camera::setCamera(Vector4f position, Vector4f lookAtPoint, Vector4f upVector)
{
	this->position = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector.normalize();
	this->lookAtVector.normalize();

}

void camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->position = Vector4f(position,1.0);
	this->lookAtVector = Vector4f(lookAtPoint - position,1.0);
	this->upVector = Vector4f(upVector,1.0);
	this->upVector.normalize();
	this->lookAtVector.normalize();

}
// change the speed of the camera motion
int camera::updateSpeed(float speed)
{
	this->speed += speed;
	return 0;
}

float camera::getSpeed(void)
{
	return(speed);
}


int camera::changeLookAtVector(float x, float y, float z)
{
	lookAtVector.x = x;
	lookAtVector.y = y;
	lookAtVector.z = z;
	lookAtVector.w = 1;

	return 0;
}


int camera::setPerspectiveView(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	return 0;
}

/**************************************************************************************************/
// zoom in by changing the field of view
int camera::zoomIn(void)
{
	zoomIn(1);
	return 0;
}


int camera::zoomIn(float zoom)
{
	fieldOfView -= zoom;
	if (fieldOfView <10) fieldOfView = 10;
	return 0;
}

int camera::zoomOut(void)
{
	zoomOut(1);
	return 0;
}


int camera::zoomOut(float zoom)
{
	fieldOfView += zoom;
	if (fieldOfView > 150) fieldOfView = 150;
	return 0;
}


/**************************************************************************************************/


Vector4f camera::getLookAtVector(void)
{
	return(this->lookAtVector);
}
