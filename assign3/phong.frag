/**
 * Phong lighting model - fragment shader.
 * Ryan Seys - 100817604
 */
#version 120
varying vec3 fragment_position;   // The fragment position interpolated from the vertex shader
varying vec4 vtx_normal;
varying vec4 color;

varying vec4 N;
varying vec4 v;

uniform mat4 modelViewProjMat;
uniform mat4 normalMat;

uniform vec4 ambientMat; // have to still pass these in
uniform vec4 diffuseMat; // pass this in
uniform vec4 specMat; // pass this in
uniform float specPow; // pass this in

void main (void) {
  vec4 diffuse;
  vec4 spec;
  vec4 ambient;

  vec4 L = normalize(vec4(100, 30, 0, 0) - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  ambient = ambientMat;

  diffuse = clamp( diffuseMat * max(dot(N,L), 0.0)  , 0.0, 1.0 ) ;
  spec = clamp( specMat * pow(max(dot(R,E),0.0),0.3*specPow) , 0.0, 1.0 );

  gl_FragColor = ambient + diffuse + spec;
}
