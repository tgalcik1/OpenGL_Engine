#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#ifndef OBJECT_HPP
#define OBJECT_HPP

class Object{
public:
    Object();
    ~Object();
    void initShaders();
    void draw();

private:
    GLuint programID;
    int width, height;

};

#endif