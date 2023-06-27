#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#include "object.hpp"

#ifndef GL_APP
#define GL_APP

class GLapp{
public:
    GLapp();
    ~GLapp();
    GLapp(int width, int height);
    int initWindow();
    void initShaders();
    void render();

    int width, height;
    GLFWwindow* window;
    std::vector<class Object*> objects;
};

#endif