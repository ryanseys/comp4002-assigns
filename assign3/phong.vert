/**
 * Phong lighting model - vertex shader.
 * Ryan Seys - 100817604
 */
#version 120
attribute vec4 vertex_position;   // the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;   // the vertex normal (in the local space) from VBO

varying vec4 color;
varying vec4 N;
varying vec4 v;

uniform mat4 modelViewProjMat;
uniform mat4 normalMat;
uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;
uniform float specPow;

void main(void) {
  v = vec4(modelViewProjMat * vertex_position);
  N = normalize(normalMat * vertex_normal);
  gl_Position = modelViewProjMat * vertex_position;
}
