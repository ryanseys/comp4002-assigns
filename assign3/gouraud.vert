/**
 * Gouraud lighting model - vertex shader.
 * Ryan Seys - 100817604
 */
#version 120

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 normalMat;

attribute vec4 vertex_position;   // the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;   // the vertex normal (in the local space) from VBO

varying vec3 fragment_position;   // The fragment position output into the fragment shader
varying vec4 fragment_normal;   // The fragment normal output into the fragment shader
varying vec4 vtx_normal;
varying vec4 color;
varying vec4 N;
varying vec4 v;
varying vec4 ambient;
varying vec4 diffuse;
varying vec4 spec;

uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;
uniform float specPow;

void main() {
  vec4 ambientMatTemp = vec4(0.8, 0.4, 0.2, 1.0);
  vec4 diffuseMatTemp = vec4(0.75, 0.75, 0.5, 1.0);
  vec4 specMatTemp = vec4(0.8, 0.8, 0.8, 1.0);
  float specPow = 5;
  vec4 L;

  mat4 modelViewMat = viewMat * modelMat;
  mat4 modelViewProjMat = projMat * modelViewMat;

  gl_Position = modelViewProjMat * vertex_position;

  v = vec4(modelViewMat * vertex_position);
  // gl_Position.w = 1.0;
  fragment_position = vec3(gl_Position);

  fragment_normal = modelViewProjMat * vertex_normal;
  vtx_normal = vertex_normal; // added for funky colors

  // "gl_NormalMatrix", it's defined as "the transpose of the inverse of the gl_ModelViewMatrix"
  N = normalize(normalMat * vertex_normal);
  L = normalize(vec4(150, 60, -100, 1.0) - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  ambient = ambientMatTemp;
  diffuse = clamp( diffuseMatTemp * max(dot(N,L), 0.0), 0.0, 1.0 ) ;
  spec = clamp ( specMatTemp * pow(max(dot(R, E),0.0),0.3*specPow) , 0.0, 1.0 );
  color = ambient + diffuse + spec;
}
