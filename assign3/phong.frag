/**
 * Phong lighting model - fragment shader.
 * Ryan Seys - 100817604
 */
#version 120
// material color components
uniform vec4 materialAmb;
uniform vec4 materialDiff;
uniform vec4 materialSpec;

// light components
uniform vec4 lightAmb;
uniform vec4 lightDiff;
uniform vec4 lightSpec;
uniform vec4 lightPos;

// Specular power (shininess factor)
uniform float specPow;

varying vec4 v; // from vertex shader
varying vec4 N; // from vertex shader

void main (void) {
  float specPow = 5;
  vec4 L = normalize(lightPos - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  vec4 ambient = lightAmb * materialAmb;
  vec4 diffuse = clamp( max(dot(N, L) * lightDiff * materialDiff, 0.0), 0.0, 1.0 ) ;
  vec4 specular = clamp (lightSpec * materialSpec * pow(max(dot(R, E), 0.0), specPow) , 0.0, 1.0 );

  gl_FragColor = ambient + diffuse + specular;
}
