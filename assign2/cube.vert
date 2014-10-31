#version 120
attribute vec4 position;
attribute vec4 in_colour;
varying vec4 out_colour;

void main(void) {
  gl_Position = position; // gl_Position is built in variable
  out_colour = in_colour;
}
