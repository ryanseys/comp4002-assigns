/**
 * Camera class originally developed by Doron Nussbaum.
 *
 * Has been heavily modified by Ryan Seys - 100817604.
 */

#include "stdlib.h"
#include "stdio.h"
#include "ryan_camera.h"

/**
 * Camera constructor
 */
camera::camera(void): position(50.0,150.0,50.0), lookAtVector(100.0,0.0,100.0), upVector(0.0,1.0,0.0), speed(0) {
  // construct the camera
}

/**
 * camera destructor
 */
camera::~camera(void) {
  // destruct the camera
}

/**
 * Change the orientation of the camera (roll transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int camera::roll(float angleDeg) {
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
int camera::pitch(float angleDeg) {
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
int camera::yaw(float angleDeg) {
  Vector3f rotVector(0.0,0.0,0.0);

  // TODO: ADD CODE

  return 0;
}

/**
 * Obtains the current position of the camera.
 *
 * @return  Camera current position.
 */
Vector3f camera::getPosition(void) {
  return (position);
}

/**
 * Obtains the lookAt point of the camera.
 *
 * @return camera lookAt point
 */
Vector3f camera::getLookAtPoint(void) {
  return (position + lookAtVector);
}

/**
 * Obtains the camera's up vector.
 *
 * @return  Camera upVector.
 */
Vector3f camera::getUpVector(void) {
  return (upVector);
}

/**
 * Changes the camera's position relative to its current position.
 * @param  dx [description]
 * @param  dy [description]
 * @param  dz [description]
 * @return    0 - success
 */
int camera::changePoitionDelta(float dx, float dy, float dz) {
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
int camera::changePositionDelta(Vector3f *dv) {
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
int camera::changeAbsPoition(float x, float y, float z) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position to a new position.
 * @param  v [description]
 * @return   0 - success
 */
int camera::changeAbsPosition(D3DXVECTOR3 *v) {
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
Vector3f camera::moveForward(float numUnits) {
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
int camera::updateOrientation(D3DXVECTOR3 rotVector, float angleRad) {

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
Matrix4f * camera::getViewMatrix() {
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
void camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector) {
  this->position = position;
  this->lookAtVector = lookAtPoint - position;
  this->upVector = upVector;
  this->upVector.noralize();
  this->lookAtVector.normalze();

}

/**
 * Change the speed of the camera motion.
 *
 * @param  speed Speed of the camera
 * @return       0 - success
 */
int camera::updateSpeed(float speed) {
  this->speed += speed;
  return 0;
}

float camera::getSpeed(void) {
  return(speed);
}
