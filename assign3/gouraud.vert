/**
 * Gouraud lighting model - vertex shader.
 * Ryan Seys - 100817604
 */
#version 120

// world properties
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 normalMat;

// material color components
uniform vec4 materialAmb;
uniform vec4 materialDiff;
uniform vec4 materialSpec;


// light components
uniform vec4 lightAmb;
uniform vec4 lightDiff;
uniform vec4 lightSpec;
uniform vec4 lightPos;

attribute vec4 vPosition;   // the vertex position (in the local space) from VBO
attribute vec4 vNormal;   // the vertex normal (in the local space) from VBO

// varying vec3 fragment_position;   // The fragment position output into the fragment shader
// varying vec4 fragment_normal;   // The fragment normal output into the fragment shader
varying vec4 vtx_normal;
varying vec4 color;
varying vec4 N;
varying vec4 v;
varying vec4 ambient;
varying vec4 diffuse;
varying vec4 spec;

uniform float specPow;

void main() {
  float specPow = 1; // specular exponent = GL_SHININESS
  vec4 L;

  mat4 modelViewMat = viewMat * modelMat;
  mat4 modelViewProjMat = projMat * modelViewMat;

  // vec4 materialEmission = vec4(0.0, 0.0, 0.0, 0.0); // material emission color

  v = vec4(modelViewMat * vPosition);

  // gl_Position.w = 1.0;
  // fragment_position = vec3(gl_Position);

  // fragment_normal = modelViewProjMat * vNormal;
  // vtx_normal = vNormal; // added for funky colors

  // NormalMatrix is the transpose of the inverse of the ModelViewMatrix
  N = normalize(normalMat * vNormal);
  L = normalize(lightPos - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));
  //
  // diffuse = vec4(diffuse_intensity, diffuse_intensity, diffuse_intensity, 0.5);

  // ambient = lightAmb * materialAmb;
  diffuse = clamp( max(dot(N, L) * lightDiff, 0.0), 0.0, 1.0 ) ;
  // spec = clamp (lightSpec * materialSpec * pow(max(dot(R, E), 0.0), specPow) , 0.0, 1.0 );
  color = diffuse;

  gl_Position = projMat * viewMat * modelMat * vPosition;
}
