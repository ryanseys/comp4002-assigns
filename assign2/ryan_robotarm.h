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
GLfloat SPHERE_RADIUS = 0.25;
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

public:
  RobotArm():
    arm1(new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH)),
    arm2(new SolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS)),
    arm3(new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH)),
    arm4(new SolidSphere(SPHERE_RADIUS, SPHERE_SLICES, SPHERE_STACKS)),
    arm5(new SolidCube(CUBE_WIDTH, CUBE_HEIGHT, CUBE_DEPTH)) {
    arm5YawAngle = 0.0;
    arm3YawAngle = 0.0;
    arm1YawAngle = 0.0;
    arm2PitchAngle = 0.0;
    arm4PitchAngle = 0.0;
  }

  void draw(GLfloat x, GLfloat y, GLfloat z) {
    glPushMatrix(); // arm start
    glTranslatef(x, y, z);

    // start arm 5
    arm5->rotate(arm5YawAngle);
    arm5->draw(0.0, 4.0, 0.0);

    // start arm 4
    glPushMatrix();
    // apply pitch around x-axis
    arm4->pitch(Vector3f(0.25, 1.75, 0.25), Vector3f(1.0, 0.0, 0.0), arm4PitchAngle);
    arm4->draw(0.25, 1.75, 0.25, 0.0);

    // start arm 3
    glPushMatrix();
    // apply arm 3 yaw
    arm3->rotate(arm3YawAngle);
    arm3->draw(0.0, 2.0, 0.0);

    // start arm 2
    glPushMatrix();
    // apply pitch around x-axis
    arm2->pitch(Vector3f(0.25, 0.75, 0.25), Vector3f(1.0, 0.0, 0.0), arm2PitchAngle);
    arm2->draw(0.25, 0.75, 0.25, 0.0);

    // start arm 1
    glPushMatrix();
    // apply arm 1 yaw
    arm1->rotate(arm1YawAngle);
    arm1->draw(0.0, 0.0, 0.0);

    glPopMatrix(); // arm 1 done
    glPopMatrix(); // arm 2 done
    glPopMatrix(); // arm 3 done
    glPopMatrix(); // arm 4 done
    glPopMatrix(); // arm 5 done

    glPopMatrix(); // arm done
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
};

#endif
