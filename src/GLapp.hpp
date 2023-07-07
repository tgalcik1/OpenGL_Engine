#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.hpp"

#ifndef GL_APP
#define GL_APP

class GLapp{
public:
    GLapp();
    ~GLapp();
    GLapp(int width, int height);
    void initWindow();
    void pollInputs(float deltaTime);
    void render();

    int width, height;
    GLFWwindow* window;
    double previousTime;

    Camera* camera;
    std::vector<class Object*> objects;
    class Light* light;

    bool wireframe;
    int prevState;
};

#endif