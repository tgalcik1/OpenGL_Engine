#include <glm/glm.hpp>

#ifndef LIGHT_HPP
#define LIGHT_HPP

class Light{
public:
    Light();
    Light(glm::vec3 position);
    Light(glm::vec3 position, glm::vec3 color, float intensity);

    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

#endif