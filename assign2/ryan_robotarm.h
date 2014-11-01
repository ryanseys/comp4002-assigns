/**
 * This robot arm class is developed by Ryan Seys.
 * It represents a hierarchical robot arm consisting of 5 parts.
 */

#ifndef RYAN_ROBOTARM
#define RYAN_ROBOTARM
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "ryan_sphere.h"
#include "ryan_cube.h"
#include "ryan_vector.h"

GLfloat CUBE_WIDTH = 0.5;
GLfloat CUBE_HEIGHT = 0.5;
GLfloat CUBE_DEPTH = 0.5;
GLfloat SPHERE_RADIUS = 0.75;
GLfloat SPHERE_SLICES = 24;
GLfloat SPHERE_STACKS = 24;

class RobotArm {
protected:
  GLfloat arm5YawAngle;
  GLfloat arm4PitchAngle;
  GLfloat arm3YawAngle;
  GLfloat arm2PitchAngle;
  GLfloat arm1YawAngle;
  SolidCube * arm1;
  SolidSphere * arm2;
  SolidCube * arm3;
  SolidSphere * arm4;
  SolidCube * arm5;
  std::vector<Matrix4f> transformations;
public:
  RobotArm() {
    arm1 = new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH);
    arm2 = new SolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
    arm3 = new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH);
    arm4 = new SolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
    arm5 = new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH);
    arm5YawAngle = 0.0;
    arm3YawAngle = 0.0;
    arm1YawAngle = 0.0;
    arm2PitchAngle = 0.0;
    arm4PitchAngle = 0.0;
  }

  void draw(GLuint shaderProg) {
    Matrix4f armMat = Matrix4f::identity();
    int size = this->transformations.size();
    for(int i = 0; i < size; i++) {
      armMat = armMat * this->transformations.at(i);
    }

    // start arm 5
    Matrix4f arm5Mat = armMat*Matrix4f::rotateRollPitchYaw(0.0, 0.0, arm5YawAngle, true);
    arm5->applyTransformation(arm5Mat);
    arm5->applyTransformation(Matrix4f::translation(0.0, 4.0, 0.0));
    arm5->draw(shaderProg);

    Matrix4f arm4Mat = arm5Mat*Matrix4f::rotateRollPitchYaw(0.0, arm4PitchAngle, 0.0, true);
    arm4->applyTransformation(arm4Mat);
    arm4->applyTransformation(Matrix4f::translation(0.0, 3.0, 0.0));
    arm4->drawSphere(shaderProg);

    Matrix4f arm3Mat = arm4Mat*Matrix4f::rotateRollPitchYaw(0.0, 0.0, arm3YawAngle, true);

    // arm3->applyTransformation(Matrix4f::translation(0.0, 2.0, 0.0));
    arm3->applyTransformation(arm3Mat);
    // arm3->applyTransformation(Matrix4f::translation(0.0, -2.0, 0.0));
    arm3->applyTransformation(Matrix4f::translation(0.0, 2.0, 0.0));
    // arm3->applyTransformation(arm3Mat);
    arm3->draw(shaderProg);

    Matrix4f arm2Mat = arm3Mat*Matrix4f::rotateRollPitchYaw(0.0, arm2PitchAngle, 0.0, true);
    arm2->applyTransformation(arm2Mat);
    arm2->applyTransformation(Matrix4f::translation(0.0, 1.0, 0.0));
    arm2->drawSphere(shaderProg);

    Matrix4f arm1Mat = arm2Mat*Matrix4f::rotateRollPitchYaw(0.0, 0.0, arm1YawAngle, true);
    arm1->applyTransformation(arm1Mat);
    arm1->applyTransformation(Matrix4f::translation(0.0, 0.0, 0.0));
    // arm1->applyTransformation(arm1Mat);
    arm1->draw(shaderProg);

    this->clear();
  }

  void rotatePart(GLint partNum, GLfloat degrees) {
    switch(partNum) {
      case 5: {
        arm5YawAngle += degrees;
        break;
      }
      case 4: {
        arm4PitchAngle += degrees;
        break;
      }
      case 3: {
        arm3YawAngle += degrees;
        break;
      }
      case 2: {
        arm2PitchAngle += degrees;
        break;
      }
      case 1: {
        arm1YawAngle += degrees;
        break;
      }
    }
  }

  void applyTransformation(Matrix4f m) {
    this->transformations.push_back(m);
  }

  void clear() {
    this->transformations.clear();
  }
};

#endif
