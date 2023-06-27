#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.c>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifndef GL_APP
#define GL_APP

class GLapp{
    public:
        GLapp();
        GLapp(int width, int height);
        int initWindow();
        void initShaders();
        void render();

    private:
        GLFWwindow* window;
        int width, height;
        GLuint programID; // this should be per object eventually
};

#endif