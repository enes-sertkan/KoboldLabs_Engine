#include "BaseGLApp.h"
#include <cstdio>


BaseGLApp::BaseGLApp(int width, int height, const char* title) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!m_window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return;
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

BaseGLApp::~BaseGLApp() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void BaseGLApp::Run() {
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(m_window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        OnFrame(deltaTime);

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}