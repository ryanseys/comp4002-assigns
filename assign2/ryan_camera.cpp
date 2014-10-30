/**
 * Camera class originally developed by Doron Nussbaum.
 *
 * Has been heavily modified by Ryan Seys - 100817604.
 */

#include "stdlib.h"
#include "stdio.h"

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ryan_camera.h"

/**
 * Camera constructor
 */
Camera::Camera(void) {
  Vector3f position(50.0,150.0,50.0);
  Vector3f lookAtVector(100.0,0.0,100.0);
  Vector3f upVector(0.0,1.0,0.0);
  speed = 0;
}

/**
 * camera destructor
 */
Camera::~Camera(void) {
  // destruct the camera
}

/**
 * Change the orientation of the camera (roll transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::roll(float angleDeg) {
  Vector3f rotVector(0.0,0.0,0.0);

  // Example code
  // get rotation axis
  rotVector = lookAtVector;

  updateOrientation(rotVector, angleDeg);
  return 0;
}

/**
 * Change the orientation of the camera (pitch transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::pitch(float angleDeg) {
  Vector3f rotVector(0.0,0.0,0.0);

  // get rotation axis

  // TODO: ADD CODE

  return 0;
}

/**
 * Change the orientation of the camera (yaw transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::yaw(float angleDeg) {
  Vector3f rotVector(0.0,0.0,0.0);

  // TODO: ADD CODE

  return 0;
}

/**
 * Obtains the current position of the camera.
 *
 * @return  Camera current position.
 */
Vector3f Camera::getPosition(void) {
  return (position);
}

/**
 * Obtains the lookAt point of the camera.
 *
 * @return camera lookAt point
 */
Vector3f Camera::getLookAtPoint(void) {
  return (position + lookAtVector);
}

/**
 * Obtains the camera's up vector.
 *
 * @return  Camera upVector.
 */
Vector3f Camera::getUpVector(void) {
  return (upVector);
}

/**
 * Changes the camera's position relative to its current position.
 * @param  dx [description]
 * @param  dy [description]
 * @param  dz [description]
 * @return    0 - success
 */
int Camera::changePositionDelta(float dx, float dy, float dz) {
  position.x += dx;
  position.y += dy;
  position.z += dz;

  return 0;
}

/**
 * Changes the camera's position relative to its current position.
 * @param  dv [description]
 * @return    0 - success
 */
int Camera::changePositionDelta(Vector3f *dv) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position to a new position.
 *
 * @param  x [description]
 * @param  y [description]
 * @param  z [description]
 * @return   0 - success.
 */
int Camera::changeAbsPosition(float x, float y, float z) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position to a new position.
 * @param  v [description]
 * @return   0 - success
 */
int Camera::changeAbsPosition(Vector3f *v) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position along the LootAt vector
 * Move the camera forward by the numUnits along the looAtVector.
 *
 * @param  numUnits [description]
 * @return          The new position.
 */
Vector3f Camera::moveForward(float numUnits) {
  // TODO: Add Code

  return (position);
}

/**
 * Update the camera orientation.
 *
 * @param  rotVector [description]
 * @param  angleRad  [description]
 * @return           [description]
 */
int Camera::updateOrientation(Vector3f rotVector, float angleRad) {

  // TODO: Add Code

  Vector3f xaxis(0.0,0.0,0.0);

  // create rotation matrix around a vector
  // rotate the camera (up vector and/or looAtVector)
  // update the  look-at and the up vectors using the x-axis vector
  // normalize the up  and look-at vectors

  return 0;
}

/**
 * Obtains the view transformation matrix.
 *
 * @return The transformation matrix
 */
Matrix4f Camera::getViewMatrix() {
  Matrix4f m1;

  // TODO: Add Code

  return(m1);
}

/**
 * Set the camera parameters.
 * @param position    [description]
 * @param lookAtPoint [description]
 * @param upVector    [description]
 */
void Camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector) {
  this->position = position;
  this->lookAtVector = lookAtPoint - position;
  this->upVector = upVector;
  this->upVector.normalize();
  this->lookAtVector.normalize();

  gluLookAt(
    position.x,
    position.y,
    position.z,
    lookAtPoint.x,
    lookAtPoint.y,
    lookAtPoint.z,
    upVector.x,
    upVector.y,
    upVector.z
  );
}

// void Camera::refresh() {

// }

/**
 * Change the speed of the camera motion.
 *
 * @param  speed Speed of the camera
 * @return       0 - success
 */
int Camera::updateSpeed(float speed) {
  this->speed += speed;
  return 0;
}

float Camera::getSpeed(void) {
  return(speed);
}
