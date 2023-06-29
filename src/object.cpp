#include <cstring>

#include "object.hpp"
#include "shader.hpp"
#include "GLapp.hpp"

using namespace glm;

// default constructor - no model given, create a cube
Object::Object(){
    //create and compile GLSL program from shaders
    shaderID = LoadShaders("shaders/object.vert", "shaders/object.frag");
    glUseProgram(shaderID);

    // create VAO
    glGenVertexArrays(1, &varrayID);
    glBindVertexArray(varrayID);

    // load default cube
    loadModel("models/cube.obj");
    ModelFromWorld = mat4(1);

    // create VBO
    glGenBuffers(1, &vbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vbufferID);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(vec3), &vert[0], GL_STATIC_DRAW);

    glGenBuffers(1, &nbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, nbufferID);
    glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(vec3), &norm[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);
}

// overloaded constructor - given filepath to a model
Object::Object(const char* path){
    // create and compile GLSL program from shaders
    shaderID = LoadShaders("shaders/object.vert", "shaders/object.frag");
    glUseProgram(shaderID);

    // create VAO
    glGenVertexArrays(1, &varrayID);
    glBindVertexArray(varrayID);

    // load model
    loadModel(path);
    ModelFromWorld = mat4(1);

    // create VBO
    glGenBuffers(1, &vbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vbufferID);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(vec3), &vert[0], GL_STATIC_DRAW);

    glGenBuffers(1, &nbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, nbufferID);
    glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(vec3), &norm[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);
}

// overloaded constructor - given filepath to a model and shaderID
Object::Object(const char* path, GLuint id){
    // create and compile GLSL program from shaders
    shaderID = id;
    glUseProgram(id);

    // create VAO
    glGenVertexArrays(1, &varrayID);
    glBindVertexArray(varrayID);

    // load model
    loadModel(path);
    ModelFromWorld = mat4(1);

    // create VBO
    glGenBuffers(1, &vbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vbufferID);
    glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(vec3), &vert[0], GL_STATIC_DRAW);

    glGenBuffers(1, &nbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, nbufferID);
    glBufferData(GL_ARRAY_BUFFER, norm.size() * sizeof(vec3), &norm[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(vec2), &uv[0], GL_STATIC_DRAW);
}

// destructor
Object::~Object(){

}

// model loader - supports .obj
bool Object::loadModel(const char* path){
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("failed to open file\n");
        return false;
    }

    while( 1 ){
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i=0; i<vertexIndices.size(); i++){
        unsigned int vertexIndex = vertexIndices[i];
        vert.push_back(temp_vertices[vertexIndex-1]);

        unsigned int normalIndex = normalIndices[i];
        norm.push_back(temp_normals[normalIndex-1]);

        unsigned int uvIndex = uvIndices[i];
        uv.push_back(temp_uvs[uvIndex-1]);
    }

    return true;
}

// update shader uniforms
void Object::updateShaders(GLapp* app){
    // model view projection matrix
    mat4 ProjFromWorld = app->camera->ProjFromWorld;

    // pass model view projection matrix as a uniform to shaders
    GLuint projFromWorldID = glGetUniformLocation(shaderID, "ProjFromWorld");
    glUniformMatrix4fv(projFromWorldID, 1, GL_FALSE, &ProjFromWorld[0][0]);

    // pass model from world matrix as a uniform to shaders
    GLuint modelFromWorldID = glGetUniformLocation(shaderID, "ModelFromWorld");
    glUniformMatrix4fv(modelFromWorldID, 1, GL_FALSE, &ModelFromWorld[0][0]);

    // pass screen resolution as a uniform to shaders
    GLuint resolution = glGetUniformLocation(shaderID, "resolution");
    glUniform2f(resolution, app->width, app->height);

    // pass current time as a uniform to shaders
    GLfloat time = glGetUniformLocation(shaderID, "time");
    glUniform1f(time, glfwGetTime());
}

// draw buffer contents to the screen
void Object::draw(GLapp* app){
    // update uniforms
    updateShaders(app);

    // draw
    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbufferID);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, nbufferID);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : uvs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glVertexAttribPointer(
        2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, vert.size());
}

void Object::translate(vec3 pos){
    ModelFromWorld[3] = vec4(pos, 1.0);
}

void Object::scale(vec3 scale){
    ModelFromWorld *= vec4(scale, 1.0);
}