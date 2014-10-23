#version 120
varying vec4 out_colour;
// float luminance;

void main(void) {
  // luminance = out_colour.r * 0.35 + out_colour.g * 0.59 + out_colour.b * 0.12;
  // gl_FragColor = vec4(luminance, luminance, luminance, 1.0);
  gl_FragColor = out_colour;
}
