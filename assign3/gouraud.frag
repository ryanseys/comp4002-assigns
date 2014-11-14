/**
 * Gouraud lighting model - fragment shader.
 * Ryan Seys - 100817604
 */
#version 120
varying vec4 color;

void main() {
  gl_FragColor = color;
}
