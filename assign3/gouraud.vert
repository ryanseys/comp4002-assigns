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

uniform float specPow;

void main() {
  float specPow = 10; // specular exponent = GL_SHININESS

  mat4 modelViewMat = viewMat * modelMat;
  mat4 modelViewProjMat = projMat * modelViewMat;
  vec4 v = vec4(modelViewMat * vPosition);

  // NormalMatrix is the transpose of the inverse of the ModelViewMatrix
  vec4 N = normalize(normalMat * vNormal);
  vec4 L = normalize(lightPos - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  vec4 ambient = lightAmb * materialAmb;
  vec4 diffuse = clamp( max(dot(N, L) * lightDiff * materialDiff, 0.0), 0.0, 1.0 ) ;
  vec4 specular = clamp (lightSpec * materialSpec * pow(max(dot(R, E), 0.0), specPow) , 0.0, 1.0 );

  // Exported
  color = ambient + diffuse + specular;
  gl_Position = projMat * viewMat * modelMat * vPosition;
}
