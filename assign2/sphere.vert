attribute vec4 vertex_position;   // the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;   // the vertex normal (in the local space) from VBO

varying vec3 fragment_position;   // The fragment position output into the fragment shader
varying vec4 fragment_normal;   // The fragment normal output into the fragment shader

uniform mat4 modelViewProjMat;


void main()
{
  fragment_position = vec3(vertex_position);
  fragment_normal   = vertex_normal;

  gl_Position = modelViewProjMat * vertex_position;
  fragment_position = vec3(gl_Position);
  // fragment_normal   = modelViewProjMat * vertex_normal;
}
