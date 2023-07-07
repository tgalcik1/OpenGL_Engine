#version 330 core

in vec4 position;
in vec3 normal;
in vec2 uv;

uniform mat4 ProjFromWorld;
uniform mat4 ModelFromWorld;
uniform vec3 lightDir;

uniform vec2 resolution;
uniform float time;

uniform vec2 tile;

uniform sampler2D textureSampler;

out vec4 fragColor;

void main(){
  vec2 st = gl_FragCoord.xy/resolution;

  // diffuse (albedo) - either gray or sampled from texture
  vec3 color = vec3(.5);
  if (textureSize(textureSampler,0) != ivec2(1,1))
    color = texture(textureSampler, uv * tile).rgb;

  // apply ambient and diffuse
  vec3 N = normalize(normal);             // surface normal
  vec3 L = normalize(lightDir);           // light direction
  color = color * (max(0., dot(N,L)) + 0.5);

  // final color
  fragColor = vec4(color, 1.0);
}