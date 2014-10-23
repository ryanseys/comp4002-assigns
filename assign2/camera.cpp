
//=============================================================================
// camera.cpp 
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


#include "stdlib.h"
#include "stdio.h"
#include "camera.h"



/******************************************************************/
/*
Purpose: camera constructor


Descripton: 

Return:


*/

camera::camera(void): position(50.0,150.0,50.0), lookAtVector(100.0,0.0,100.0), upVector(0.0,1.0,0.0)
, speed(0)
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
	Vector3f rotVector(0.0,0.0,0.0);

	// Example code
	// get rotation axis
	rotVector = lookAtVector;

	updateOrientation(rotVector, angleDeg);


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
	Vector3f rotVector(0.0,0.0,0.0);

	// get rotation axis

	// ADD CODE


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
	Vector3f rotVector(0.0,0.0,0.0);
	
	// ADD CODE

	return 0;
}

/******************************************************************/
/*
Purpose: obtains the current position of the camera


Descripton: 

Return:
D3DXVECTOR3 - camera position

*/

Vector3f camera::getPosition(void)
{
	return (position);
}


/******************************************************************/
/*
Purpose: obtains the lookAt point of the camera


Descripton: 

Return:
D3DXVECTOR3 - camera lookAt point

*/


Vector3f camera::getLookAtPoint(void)
{
	return (position + lookAtVector);
}

/******************************************************************/
/*
Purpose: obtains the camera's up vector


Descripton: 

Return:
D3DXVECTOR3 - camera upVector

*/



Vector3f camera::getUpVector(void)
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
	return 0;
}

/******************************************************************/
/*
Purpose: changes the camera's position to a new position


Descripton: 

Return:
0 - success

*/

int camera::changeAbsPosition(D3DXVECTOR3 *v)
{
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
	// Add Code

	return (position);
}

/******************************************************************/
/*
Purpose: 


Descripton: 

Return:


*/


int camera::updateOrientation(D3DXVECTOR3 rotVector, float angleRad)
{

	// Add Code

	Vector3f xaxis(0.0,0.0,0.0);

	// create rotation matrix around a vector
	
	// rotate the camera (up vector and/or looAtVector)

	// update the  look-at and the up vectors using the x-axis vector


	// normalize the up  and loo-at vectors


	return 0;
}

/******************************************************************/
/*
Purpose: obtains the view transformation matrix


Descripton: 

Return:
the transformation matrix

*/



Matrix4f * camera::getViewMatrix()
{
	
	Matrix4f m1;
	
	
	// Add Code

	return(m1);
}

/******************************************************************/
/*
Purpose: set the camera parameters


Descripton: 

Return:


*/


void camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->position = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector.noralize();
	this->lookAtVector.normalze();

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
