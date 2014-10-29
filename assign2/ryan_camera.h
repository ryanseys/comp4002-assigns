#ifndef RYAN_CAMERA
#define RYAN_CAMERA

const int NUM_DIM = 3;

class Camera {

protected:
  GLfloat lookAtVector[NUM_DIM] = {0.0, 0.0, 0.0};
  GLfloat upVector[NUM_DIM] = {0.0, 0.0, 0.0};
public:
  Camera(GLfloat w, GLfloat h, GLfloat d) {

  }

  void pitch(GLfloat angle) {
    // rotate around x axis
  }

  void yaw(GLfloat angle) {
    // rotate around y axis
  }

  void roll(GLfloat angle) {
    // rotate around z axis
  }
};

#endif
