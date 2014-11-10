/**
 * Phong lighting model - fragment shader.
 * Ryan Seys - 100817604
 */
#version 120
varying vec3 fragment_position;   // The fragment position interpolated from the vertex shader
varying vec4 fragment_normal;   // The fragment normal interpolated from the vertex shader
varying vec4 vtx_normal;
varying vec4 color;

void main() {
  // gl_FragColor = vec4(vtx_normal.x, vtx_normal.y, vtx_normal.z, 0.0);
  gl_FragColor = color;
}
