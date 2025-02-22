#ifndef GRID_HPP
#define GRID_HPP

#include <glm/glm.hpp>
#include <vector>
#include <GL/glew.h> // Include GLEW for OpenGL types

class Grid {
public:
    Grid(float size, float spacing, glm::vec3 color, GLuint shaderProgram);
    ~Grid();

    void updateGrid(float newSize, float newSpacing);
    void draw(const glm::mat4& viewProjectionMatrix);
    void toggleVisibility();

    void setColor(const glm::vec3& newColor);
    void setSize(float newSize);
    void setSpacing(float newSpacing);

    std::vector<glm::vec3> vertices; // Grid vertices

private:
    void generateGridVertices();
    void updateVBO();

    GLuint vao; // Vertex Array Object
    GLuint vbo; // Vertex Buffer Object
    GLuint shaderProgram; // Shader program for grid rendering

    float size;    // Grid size (width and height)
    float spacing; // Distance between grid lines
    glm::vec3 color; // Grid line color
    bool isVisible; // Grid visibility
};

#endif // GRID_HPP