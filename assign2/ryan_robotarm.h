#ifndef RYAN_ROBOTARM
#define RYAN_ROBOTARM
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "ryan_sphere.h"
#include "ryan_cube.h"
/**
 * This robot arm class developed by Ryan Seys.
 */

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
    glPushMatrix(); // apply arm 5 yaw
    glTranslatef(CUBE_WIDTH/2, 0.0, CUBE_DEPTH/2); // half of the cube width and height
    glRotatef(arm5YawAngle += 1.0, 0.0, 1.0, 0.0);
    glTranslatef(-(CUBE_WIDTH/2), 0.0, -(CUBE_DEPTH/2)); // inverse transformation
    arm5->draw(0.0, 4.0, 0.0);

    // start arm 4
    glPushMatrix();
    // TODO: apply arm 4 pitch
    // glTranslatef(0.0, -1.75, 0.0);
    // glRotatef(arm4PitchAngle += 1.0, 0.0, 0.0, 0.0);
    // glTranslatef(0.0, 1.75, 0.0);
    arm4->draw(0.25, 1.75, 0.25, 0.0);

    // start arm 3
    glPushMatrix();
    // apply arm 3 yaw
    glTranslatef(CUBE_WIDTH/2, 0.0, CUBE_DEPTH/2); // half of the cube width and height
    glRotatef(arm3YawAngle += 1.0, 0.0, 1.0, 0.0);
    glTranslatef(-(CUBE_WIDTH/2), 0.0, -(CUBE_DEPTH/2)); // inverse transformation
    arm3->draw(0.0, 2.0, 0.0);

    glPushMatrix();
    // TODO: apply arm 2 pitch
    arm2->draw(0.25, 0.75, 0.25, 0.0);
    glPushMatrix(); // apply arm 1 yaw

    glTranslatef(CUBE_WIDTH/2, 0.0, CUBE_DEPTH/2); // half of the cube width and height
    glRotatef(arm1YawAngle += 1.0, 0.0, 1.0, 0.0);
    glTranslatef(-(CUBE_WIDTH/2), 0.0, -(CUBE_DEPTH/2)); // inverse transformation
    arm1->draw(0.0, 0.0, 0.0);

    glPopMatrix(); // arm 1 done
    glPopMatrix(); // arm 2 done
    glPopMatrix(); // arm 3 done
    glPopMatrix(); // arm 4 done
    glPopMatrix(); // arm 5 done

    glPopMatrix(); // arm done
  }

  void drawPart(GLint partNum) {
    printf("Drawing part %d\n", partNum);
  }
};

#endif
