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
    Object(const char* texturePath, const char* vertShaderPath, const char* fragShaderPath);
    Object(const char* modelPath, const char* texturePath, const char* vertShaderPath, const char* fragShaderPath);
    ~Object();
    GLuint getShaderID(){ return shaderID; };
    bool loadModel(const char* path);
    GLuint loadTexture(const char* path);
    void updateShaders(class GLapp* app);
    void draw(class GLapp* app);

    // transformations
    void translate(glm::vec3 pos);
    void scale(glm::vec3 scale);
    void rotate(glm::vec3 axis, float angle);

    // texture adjustments
    void setTiling(glm::vec2 tile);

private:
    GLuint varrayID;    // VAO
    GLuint vbufferID;   // VBO
    GLuint nbufferID;   // VBO
    GLuint uvbufferID;   // VBO

    GLuint shaderID;    // shader program
    GLuint textureID;

    std::vector<glm::vec3> vert;    // per-vertex position
    std::vector<glm::vec3> norm;    // per-vertex normal
    std::vector<glm::vec2> uv;      // per-vertex uv coordinates

    glm::mat4 ModelFromWorld;

    glm::vec2 tiling;
};

#endif