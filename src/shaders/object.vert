#version 330 core

layout(location = 0)
in vec3 vertexPosition_modelspace;
uniform mat4 MVP;
uniform float time;

void main(){
  vec4 pos = MVP * vec4(vertexPosition_modelspace,1);
  pos.x += sin(time);
  pos.y += cos(time);
  gl_Position = pos;
}