#include "GLapp.hpp"
#include "object.hpp"
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

GLapp::GLapp(int w, int h){
    // initialize app and GLFW window
    width = w;
    height = h;
    wireframe = false;
    prevState = GLFW_RELEASE;
    initWindow();
    
    // tell OpenGL to enable z-buffer for overlapping surfaces
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    // backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // lock frame rate to display
    glfwSwapInterval(1);
}

GLapp::~GLapp(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void GLapp::initWindow(){
    // initialize GLFW
    if(!glfwInit())
    {
        fprintf(stderr, "failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // to make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // we don't want the old OpenGL 
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // non-resizable window

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(width, height, "OpenGL program", nullptr, nullptr);
    if(window == nullptr){
        fprintf(stderr, "failed to open GLFW window\n");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window); 
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);  // ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);  // hide cursor
    
    // initialize GLEW
    glewExperimental = true; // needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "failed to initialize GLEW\n");
    }

    glViewport(0, 0, width, height);
}

void GLapp::pollInputs(float deltaTime){
    // get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    // reset mouse position for next frame
    glfwSetCursorPos(window, width/2, height/2);

    // compute new orientation
    camera->horizontalAngle += camera->mouseSpeed * deltaTime * float(width/2 - xpos);
    camera->verticalAngle += camera->mouseSpeed * deltaTime * float(height/2 - ypos);

    camera->forward = vec3(cos(camera->verticalAngle) * sin(camera->horizontalAngle),
                           sin(camera->verticalAngle),
                           cos(camera->verticalAngle) * cos(camera->horizontalAngle));
    camera->right = vec3(sin(camera->horizontalAngle - 3.14f/2.0f),
                         0,
                         cos(camera->horizontalAngle - 3.14f/2.0f));
    camera->up = cross(camera->right, camera->forward);

    // forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera->position += camera->forward * deltaTime * camera->speed;
    }
    // backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera->position -= camera->forward * deltaTime * camera->speed;
    }
    // right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera->position += camera->right * deltaTime * camera->speed;
    }
    // left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera->position -= camera->right * deltaTime * camera->speed;
    }
    // up
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        camera->position += camera->up * deltaTime * camera->speed;
    }
    // down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        camera->position -= camera->up * deltaTime * camera->speed;
    }

    // update ProjFromWorld matrix
    camera->ProjFromWorld = perspective(radians(camera->fov), (float) width / (float) height, 0.1f, 1000.0f) * 
                            lookAt(camera->position, camera->position + camera->forward, camera->up);

    // toggle wireframe drawing
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && prevState == GLFW_RELEASE){
        wireframe = !wireframe;
        prevState = GLFW_PRESS;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE){
        prevState = GLFW_RELEASE;
    }
}

void GLapp::render(){
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - previousTime;

    pollInputs((float)deltaTime);

    // clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw each object
    for (auto obj : objects){
        obj->draw(this);
    }

    // display contents to the window
    glfwSwapBuffers(window);
    previousTime = currentTime;
}

int main(void)
{
    // initialize OpenGL
    GLapp app(1280, 960);

    app.camera = new Camera();

    // scene objects
    app.objects.push_back(new Object("models/bunny.obj"));

    app.objects.push_back(new Object());
    app.objects.at(1)->translate(vec3(0, -1, 0));
    app.objects.at(1)->scale(vec3(10, 1, 10));

    app.objects.push_back(new Object("models/bunny.obj"));
    app.objects.at(2)->scale(vec3(2, 2, 2));
    app.objects.at(2)->translate(vec3(3, 0, 0));

    app.objects.push_back(new Object("models/bunny.obj"));
    app.objects.at(3)->scale(vec3(0.5, 0.5, 0.5));
    app.objects.at(3)->translate(vec3(-2, 0, 0));

    // reset mouse position for first frame
    glfwSetCursorPos(app.window, app.width/2, app.height/2);

    // render and poll each frame
    while (glfwGetKey(app.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(app.window) == 0){
        app.render();
        glfwPollEvents();
    }

    return 0;
}