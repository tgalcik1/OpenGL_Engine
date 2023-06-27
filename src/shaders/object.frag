#version 330 core

in vec3 fragmentColor;
uniform vec2 u_resolution;
uniform float time;
out vec4 color;

void main(){
  vec2 st = gl_FragCoord.xy/u_resolution;
  color = vec4(fragmentColor, 1.0);
}