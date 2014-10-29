#ifndef RYAN_CAMERA
#define RYAN_CAMERA

#pragma once
#include "ryan_matrix.h"

const int NUM_DIM = 3;

class Camera {

private:
  int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space

protected:
  GLfloat lookAtVector[NUM_DIM] = {0.0, 0.0, 0.0};
  GLfloat upVector[NUM_DIM] = {0.0, 0.0, 0.0};

public:
  Matrix4f rotMat;
  Vector3f position;
  Vector3f upVector;
  Vector3f lookAtVector;
  Matrix4f * getViewMatrix(D3DXMATRIX * viewMatrix);
  float speed;

  camera(void);
  ~camera(void);
  int roll(float angleDeg);
  int pitch(float angleDeg);
  int yaw(float angleDeg);
  Vector3f getPosition(void);
  Vector3f getLookAtPoint(void);
  Vector3f getUpVector(void);
  int changePoitionDelta(float dx, float dy, float dz); // change position by relative amount
  int changePositionDelta(Vector3f *dv);        // change position by relative amount
  int changeAbsPoition(float x, float y, float z);    // change to a new position in space
  int changeAbsPosition(Vector3f *v);         // change to a new position in space
  Vector3f moveForward(float numUnits);  // moves the camera forward by the numUnits units along the lookAtVector
  void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
  int updateSpeed(float speed);
  float getSpeed(void);


  Camera(GLfloat w, GLfloat h, GLfloat d) {

  }
};

#endif
