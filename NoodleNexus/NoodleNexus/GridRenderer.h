//#ifndef GRID_RENDERER_H
//#define GRID_RENDERER_H
//#define GLFW_INCLUDE_NONE
//
//#pragma once
//
//#include <gl/glew.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <vector>
//
//class GridRenderer {
//public:
//    GridRenderer();
//    ~GridRenderer();
//
//    bool Initialize();
//    void Render(const glm::mat4& viewProjectionMatrix, const glm::vec3& cameraPosition);
//
//private:
//    GLuint shaderProgram;
//    GLuint vao, vbo, ebo;
//
//    GLuint LoadShaders(const char* vertexFilePath, const char* fragmentFilePath);
//    void SetupGridVAO();
//};
//
//#endif // GRID_RENDERER_H