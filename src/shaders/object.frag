#version 330 core
uniform vec2 u_resolution;
uniform float time;
out vec4 color;

void main(){
  vec2 st = gl_FragCoord.xy/u_resolution;
  color = vec4(-sin(time), cos(time), sin(time), 1.0);
}