#include <cstring>

#include "object.hpp"
#include "shader.hpp"
#include "light.hpp"
#include "GLapp.hpp"

using namespace glm;

// default constructor - no model given create a cube
Object::Object(const char *texturePath, const char *vertShaderPath, const char *fragShaderPath){

    const char *vs = vertShaderPath == nullptr ? "shaders/object.vert" : vertShaderPath;
    const char *fs = vertShaderPath == nullptr ? "shaders/object.frag" : fragShaderPath;

    //create and compile GLSL program from shaders
    shaderID = LoadShaders(vs, fs);
    glUseProgram(shaderID);

    // create VAO
    glGenVertexArrays(1, &varrayID);
    glBindVertexArray(varrayID);

    // load default cube
    loadModel("models/cube.obj");
    loadTexture(texturePath);
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

// overloaded constructor - given filepath to a model and shader
Object::Object(const char* modelPath, const char *texturePath, const char *vertShaderPath, const char *fragShaderPath){
    const char *vs = vertShaderPath == nullptr ? "shaders/object.vert" : vertShaderPath;
    const char *fs = vertShaderPath == nullptr ? "shaders/object.frag" : fragShaderPath;

    //create and compile GLSL program from shaders
    shaderID = LoadShaders(vs, fs);

    // create VAO
    glGenVertexArrays(1, &varrayID);
    glBindVertexArray(varrayID);

    // load model
    loadModel(modelPath);
    if(!loadTexture(texturePath)){
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 0, 0, 0, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
    }
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

// texture loader - supports .bmp
GLuint Object::loadTexture(const char* imagepath){
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){printf("Image could not be opened\n"); return 0;}

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    // Create one OpenGL texture
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    tiling = vec2(1.0, 1.0);

    return textureID;
}

// update shader uniforms
void Object::updateShaders(GLapp* app){
    glUseProgram(shaderID);
    
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

    // pass light direction as a uniform to shaders
    GLuint lightDir = glGetUniformLocation(shaderID, "lightDir");
    glUniform3f(lightDir, app->light->position.x, app->light->position.y, app->light->position.z);

    // pass texture tiling and offset as vec2 uniforms
    GLuint tile = glGetUniformLocation(shaderID, "tile");
    glUniform2f(tile, tiling.x, tiling.y);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
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

void Object::rotate(vec3 axis, float angle){
    ModelFromWorld += glm::rotate(ModelFromWorld, glm::radians(angle), axis);
}

void Object::setTiling(vec2 tile){
    tiling = tile;
}