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
uniform float shinyFactor;

varying vec4 v; // from vertex shader
varying vec4 N; // from vertex shader

void main (void) {
  float specPow = 5;
  vec4 L = normalize(lightPos - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  vec4 ambient = lightAmb * materialAmb;
  vec4 diffuse = clamp(max(dot(N, L) * lightDiff * materialDiff, 0.0), 0.0, 1.0);
  vec4 specular = clamp(lightSpec * materialSpec * pow(max(dot(R, E), 0.0), specPow) , 0.0, 1.0);

  // position of spot light is set here
  vec4 spotPosition = vec4(190, 200, 190, 0);
  // spot direction vector is from position to the middle of sphere
  vec4 spotDirection = normalize(vec4(100, 10, 100, 0) - spotPosition);
  // this is the cutoff angle for the spotlight
  float spotCutoffAngle = 1; // degrees

  // float spotEffect = vec4(0, 0, 0, 0);
  float intensity = max(dot(N, -spotDirection), 0.0);
  vec4 spot = vec4(intensity, intensity, intensity, 1);
  float power = 0;
  if(intensity > 0.0) {
    float angle = acos(dot(-spotDirection, N));
    if(angle < spotCutoffAngle) {
      power = pow(90-angle, 10); // some spotExponent
    }
  }

  gl_FragColor = vec4(power, power, power, power) + ambient + diffuse + specular;
}
