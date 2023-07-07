#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Camera::Camera(){
    position = vec3(0,0,0);
    fov = 90.0f;
    speed = 3.0f;
    mouseSpeed = 0.05f;

    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;

    forward = vec3(cos(verticalAngle) * sin(horizontalAngle),
                   sin(verticalAngle),
                   cos(verticalAngle) * cos(horizontalAngle));

    right = vec3(sin(horizontalAngle - 3.14f/2.0f),
                 0,
                 cos(horizontalAngle - 3.14f/2.0f));

    up = cross(right, forward);

    ProjFromWorld = perspective(radians(fov), (float) 800.0 / (float) 600.0, 0.1f, 1000.0f) * 
                    lookAt(position, position + forward, up);
}

Camera::Camera(vec3 _position, float _fov, float _speed, float _mouseSpeed, float _horizontalAngle, float _verticalAngle){
    position = _position;
    fov = _fov;
    speed = _speed;
    mouseSpeed = _mouseSpeed;
    horizontalAngle = _horizontalAngle;
    verticalAngle = _verticalAngle;

    forward = vec3(cos(verticalAngle) * sin(horizontalAngle),
                   sin(verticalAngle),
                   cos(verticalAngle) * cos(horizontalAngle));

    right = vec3(sin(horizontalAngle - 3.14f/2.0f),
                 0,
                 cos(horizontalAngle - 3.14f/2.0f));

    up = cross(right, forward);

    ProjFromWorld = perspective(radians(fov), (float) 800.0 / (float) 600.0, 0.1f, 1000.0f) * 
                    lookAt(position, position + forward, up);
}