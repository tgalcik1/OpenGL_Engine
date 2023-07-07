#include <glm/glm.hpp>

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera{
public:
    Camera();
    Camera(glm::vec3 position, float fov, float speed, float mouseSpeed, float horizontalAngle, float verticalAngle);

    glm::vec3 position;
    glm::vec3 forward, right, up;
    glm::mat4 ProjFromWorld;

    float fov;
    float speed;
    float mouseSpeed;

    float horizontalAngle, verticalAngle;
};

#endif