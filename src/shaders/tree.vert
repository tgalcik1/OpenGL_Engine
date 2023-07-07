#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 ProjFromWorld;
uniform mat4 ModelFromWorld;

uniform float time;

out vec4 position;
out vec3 normal;
out vec2 uv;

void main(){
  uv = vUV;
  
  position = ModelFromWorld * vec4(vPosition, 1);
  //position.xy += clamp(vUV + vec2(cos(vUV.x*30.0+time*6.2831)/30.0,0), -1/vUV.y, 1/vUV.y);

  normal = normalize(vNormal * mat3(ModelFromWorld));

  gl_Position = ProjFromWorld * position;
}