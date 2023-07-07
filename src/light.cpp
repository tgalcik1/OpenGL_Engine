#include "light.hpp"

using namespace glm;

Light::Light(){
    position = vec3(0,10,0);
    color = vec3(1,1,1);
    intensity = 10.0f;
}

Light::Light(vec3 _position){
    position = _position;
    color = vec3(1,1,1);
    intensity = 10.0f;
}

Light::Light(vec3 _position, vec3 _color, float _intensity){
    position = _position;
    color = _color;
    intensity = _intensity;
}