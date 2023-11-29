#include <GL\glew.h>
//
#include "Utils/Utils.h"
#include <GLFW\glfw3.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define numVAOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint offsetLoc;
float x = 0.0f;
float inc = 0.01f;

void init(GLFWwindow* window) {
    renderingProgram = Utils::createShaderProgram(SHADERS_DIR "vertShader.glsl", SHADERS_DIR "fragShader.glsl");
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    x += inc;
    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;
    offsetLoc = glGetUniformLocation(renderingProgram, "offset");
    glProgramUniform1f(renderingProgram, offsetLoc, x);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter 2 - program 5", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}