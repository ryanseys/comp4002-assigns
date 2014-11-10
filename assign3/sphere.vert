#version 120
attribute vec4 vertex_position;   // the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;   // the vertex normal (in the local space) from VBO

varying vec3 fragment_position;   // The fragment position output into the fragment shader
varying vec4 fragment_normal;   // The fragment normal output into the fragment shader
varying vec4 vtx_normal;
varying vec4 color;
varying vec4 N;
varying vec4 v;
varying vec4 diffuse;
varying vec4 spec;

uniform mat4 modelViewProjMat;
uniform mat4 normalMat;

uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;
uniform float specPow;

void main()
{
vec4 ambientMatTemp = vec4(0.8, 0.4, 0.2, 0.0);
vec4 diffuseMatTemp = vec4(0.75, 0.75, 0.5, 0.0);
vec4 specMatTemp = vec4(0.8, 0.8, 0.8, 0.0);
vec4 ambient;
vec4 diffuse = vec4(0, 0, 0, 0);
vec4 spec = vec4(0, 0, 0, 0);
vec4 L;

  v = vec4(modelViewProjMat * vertex_position);
  gl_Position = v;
  // gl_Position.w = 1.0;
  fragment_position = vec3(gl_Position);

  fragment_normal = modelViewProjMat * vertex_normal;
  vtx_normal = vertex_normal; // added for funky colors

  // "gl_NormalMatrix", it's defined as "the transpose of the inverse of the gl_ModelViewMatrix"
  N = normalize(normalMat * vertex_normal);
  L = normalize(vec4(110, 20, 110, 0.0) - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  ambient = ambientMatTemp;
  // diffuse = clamp( diffuseMatTemp * max(dot(N,L), 0.0), 0.0, 1.0 ) ;
  // spec = clamp ( specMat * pow(max(dot(R,E),0.0),0.3*specPow) , 0.0, 1.0 );
  color = ambient + diffuse + spec;
}
