/**
 * This program is distributed for education purposes only and WITHOUT ANY WARRANTY.
 * This file is governed by the GNU General Public License.
 * Copyright 2014 Doron Nussbaum
 *
 * Cleaned and edited by Ryan Seys - 100817604
 */

#ifndef RYAN_MATRIX
#define RYAN_MATRIX

#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants
#include <math.h>
#include "assert.h"
#include "ryan_vector.h"

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))
#define RadianToDegrees(x) (float)(((x) * 180.0f / M_PI))

class Matrix4f {

public:
  float *m[4]; // can be removed
  Vector4f vm[4];

  Matrix4f() {
  int i;

  reset(0.0);
    for (i = 0; i < 4; i++) m[i]= (float *) &vm[i]; // can be removed
  }

  Matrix4f(Vector4f v0, Vector4f v1, Vector4f v2, Vector4f v3) {
    vm[0] = v0;
    vm[1] = v1;
    vm[2] = v2;
    vm[3] = v3;
  }

  // Static functions

  /**
   * Returns an identity matrix
   * @return Identity matrix
   */
  static Matrix4f identity() {
    Matrix4f m1;
    m1.reset(0);
    m1.vm[0].x = m1.vm[1].y = m1.vm[2].z = m1.vm[3].w = 1;
    return(m1);
  }

  /**
   * Creates a transpose of the matrix
   * Note: it does not change the internal values of the matrix
   * @return    A transposed matrix
   */
  static Matrix4f transpose(Matrix4f m1) {
    Matrix4f m2;

    m2.vm[0] = Vector4f(m1.vm[0].x, m1.vm[1].x, m1.vm[2].x, m1.vm[3].x);
    m2.vm[1] = Vector4f(m1.vm[0].y, m1.vm[1].y, m1.vm[2].y, m1.vm[3].y);
    m2.vm[2] = Vector4f(m1.vm[0].z, m1.vm[1].z, m1.vm[2].z, m1.vm[3].z);
    m2.vm[3] = Vector4f(m1.vm[0].w, m1.vm[1].w, m1.vm[2].w, m1.vm[3].w);

    return(m2);
  }

  /**
   * Return a scale matrix.
   * Note: Internal parameters are not changed.
   * Note: It resets the data first.
   *
   * @param  scaleX [description]
   * @param  scaleY [description]
   * @param  scaleZ [description]
   * @return        A scale matrix.
   */
  static Matrix4f scale(float scaleX, float scaleY, float scaleZ) {
    Matrix4f m1;
    identity();
    m1.vm[0].x = scaleX;
    m1.vm[1].y = scaleY;
    m1.vm[2].z = scaleZ;
    return m1;
  }

  // set the matrix as a pitch rotation matrix
  // if degree is true (!=0) then angle is given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateX(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[1].y = m1.vm[2].z=cos(angle);
    m1.vm[1].z = -sin(angle);
    m1.vm[2].y = -m1.vm[1].z;
    return(m1);
  }

  // set the matrix as a yaw rotation matrix
  // if degree is true (!=0) then angle is given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateY(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[0].x = m1.vm[2].z=cos(angle);
    m1.vm[0].z = sin(angle);
    m1.vm[2].x = -m1.vm[0].z;
    return(m1);
  }

  // set the matrix as a roll rotation matrix
  // if degree is true (!=0) then angle is given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateZ(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[0].x = m1.vm[1].y = cos(angle);
    m1.vm[0].y = -sin(angle);
    m1.vm[1].x = -m1.vm[0].y;

    return(m1);
  }

  // set the matrix as a roll pitch and yaw rotation matrix
  // the resulting matrix M=Mat(yaw)*mat(pitch)*mat(roll)
  // if degree is true (!=0) then all angles are given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateRollPitchYaw(float angleRoll, float anglePitch, float angleYaw, int degree) {
    Matrix4f m1;
    m1 = identity();
    m1=rotateY(angleYaw, degree)*rotateX(anglePitch, degree)*rotateZ(angleRoll, degree);

    return(m1);
  }

  // set the matrix as a rotation around a vector where only a rotation vector is given
  // if degree is true (!=0) then all angles are given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateVector(Vector3f, float angle, int degree) {
    Matrix4f m1;
    m1 = identity();

    // TODO: ADD CODE

    return(m1);
  }

  // set the matrix as a rotation around a vector where only a rotation vector is given
  // if degree is true (!=0) then all angles are given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateVector(Vector4f u, float angle, int degree) {
    Matrix4f m1;
    Vector3f v = Vector3f(u.x, u.y, u.z);
    m1 = rotateVector(v, angle, degree);

    return(m1);
  }

  static Matrix4f translation(float dx, float dy, float dz) {
    Matrix4f m1;

    m1 = identity();
    m1.vm[0].x = dx;
    m1.vm[1].y = dy;
    m1.vm[2].z = dz;
    return(m1);
  }

  static Matrix4f cameraMatrix(Vector3f position, Vector4f lookAtVector, Vector3f UpVector) {
    Matrix4f m1;
    m1 = identity();

    // ADD CODE

    return(m1);
  }

  static Matrix4f symmetricPerspectiveProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    Matrix4f m1;
    m1 = identity();

    // ADD CODE

    return(m1);
  }

  static Matrix4f frustumProjectionMatrix(float winMinX, float winMinY, float winMaxX,float winMaxY,float nearPlane, float farPlane) {
    Matrix4f m1;
    m1 = identity();

    // ADD CODE

    return(m1);
  }

  // print the matrix (prints the vectors)
  // friend std::ostream& operator << (std::ostream& os, const Matrix4f& m1) {
  //   int i;
  //   for (i = 0; i <=3; i++) {
  //     os <<m1.vm[i] << std::endl;
  //   }
  //   return(os);
  // }

  /**
   * Resets all the values of the matrix to "value".
   * Sets all 16 values of the matrix to value.
   * @param value Value to set all the numbers to.
   */
  inline void reset(float value){
    int i;
    for (i=0; i <=3; i++) {
      vm[i]= Vector4f(value, value, value, value);
    }
  }

  // operators

  inline Matrix4f operator*(const Matrix4f& rhs) const {
    Matrix4f m1,m2;
    int i,j;

    m2 = transpose(rhs);
    for (i = 0 ; i < 4 ; i++) {
      for (j = 0 ; j < 4 ; j++) {
        m1.m[i][j] = dot(vm[i], m2.vm[j]);
      }
    }
    return(m1);
  }

  inline Matrix4f operator*(const float f) const {
    Matrix4f m1;
    int i;

    for (i = 0 ; i < 4 ; i++) {
      m1.vm[i] = vm[i]*f;
    }

    return(m1);
  }

  friend Matrix4f operator*(const float f, Matrix4f m1) {
    return(m1*f);
  }

  inline Matrix4f operator+(const Matrix4f& rhs) const {
    Matrix4f m1, m2;
    int i;

    for (i = 0 ; i < 4 ; i++) {
      m1.vm[i] = vm[i]+rhs.vm[i];
    }

    return(m1);
  }

  inline Matrix4f operator-(const Matrix4f& rhs) const {
    Matrix4f m1, m2;
    int i;

    for (i = 0 ; i < 4 ; i++) {
      m1.vm[i] = vm[i]-rhs.vm[i];
    }

    return(m1);
  }

  // multiply the matrix by a vector
  Vector4f operator*(const Vector4f& v) const {
    Vector4f r;

    r.x = dot(vm[0],v);
    r.y = dot(vm[1],v);
    r.z = dot(vm[2],v);
    r.w = dot(vm[3],v);

    return r;
  }
};

#endif
