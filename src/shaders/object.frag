#version 330 core

in vec4 position;
in vec3 normal;
in vec2 uv;

uniform mat4 ProjFromWorld;
uniform mat4 ModelFromWorld;
uniform vec2 resolution;
uniform float time;

out vec4 fragColor;

void main(){
  //vec2 st = gl_FragCoord.xy/resolution;

  vec3 color = vec3(0.5);
  
  vec3 LightDir = vec3(0, 1, 0);

  // apply ambient and diffuse
  vec3 N = normalize(normal);             // surface normal
  vec3 L = normalize(LightDir);           // light direction
  color = color * (max(0., dot(N,L)) + 0.5);

  // final color
  fragColor = vec4(normal, 1.0);
}