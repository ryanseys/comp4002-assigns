varying vec3 fragment_position;   // The fragment position interpolated from the vertex shader
varying vec4 fragment_normal;   // The fragment normal interpolated from the vertex shader

void main() {
  gl_FragColor = vec4(fragment_normal.x, fragment_normal.y, fragment_normal.z, 0.0);
}
