#ifndef BASEGLAPP_H
#define BASEGLAPP_H
#define GLEW_NO_GLU

#include <GL/glew.h>
#include <GLFW/glfw3.h>



class BaseGLApp {
public:
    BaseGLApp(int width, int height, const char* title);
    virtual ~BaseGLApp();

    virtual void Start() = 0;
    virtual void OnFrame(double deltaTime) = 0;

    void Run();

protected:
    GLFWwindow* m_window;
};

#endif // BASEGLAPP_H