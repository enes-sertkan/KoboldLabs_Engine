#ifndef ENDLESSGRIDDEMO_H
#define ENDLESSGRIDDEMO_H

#include "BaseGLApp.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class EndlessGridDemo : public BaseGLApp {
public:
    EndlessGridDemo();
    void Start() override;
    void OnFrame(double deltaTime) override;

private:
    GLuint m_shaderProgram;
    GLuint m_vao, m_vbo, m_ebo;
    GLsizei m_indexCount;

    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    float m_cameraSpeed;

    GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource);
    void CreateGrid();
};

#endif // ENDLESSGRIDDEMO_H