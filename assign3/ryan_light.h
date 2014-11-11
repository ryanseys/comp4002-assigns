#ifndef RYAN_LIGHT
#define RYAN_LIGHT

#include "ryan_vector.h"
#include "ryan_matrix.h"

class Light {
public:
  Vector4f position;
  Vector4f ambient;
  Vector4f diffuse;
  Vector4f specular;

  Light() {
    this->position = Vector4f(0, 0, 0, 0);
    this->ambient = Vector4f(0, 0, 0, 0);
    this->diffuse = Vector4f(0, 0, 0, 0);
    this->specular = Vector4f(0, 0, 0, 0);
  }

  Light(Vector4f pos, Vector4f amb, Vector4f diff, Vector4f spec) {
    this->position = pos;
    this->ambient = amb;
    this->diffuse = diff;
    this->specular = spec;
  }

  void setAmbient(GLfloat r, GLfloat g, GLfloat b) {
    this->ambient = Vector4f(r, g, b, 0.0);
  }

  void setDiffuse(Vector4f diff) {
    this->diffuse = diff;
  }

  void setSpecular(Vector4f spec) {
    this->specular = spec;
  }
};

#endif
