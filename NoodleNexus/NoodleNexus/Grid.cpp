#include "Grid.hpp"
#include <iostream>
#include "Scene.hpp"


Grid::Grid(float size, float spacing, glm::vec3 color, GLuint shaderProgram)
    : size(size), spacing(spacing), color(color), isVisible(true), shaderProgram(shaderProgram) {
    // Generate VAO and VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Generate grid vertices
    generateGridVertices();
    updateVBO();
}

Grid::~Grid() {
    // Clean up VAO and VBO
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Grid::generateGridVertices() {
    vertices.clear();

    int numLines = static_cast<int>(size / spacing);

    // Generate horizontal lines
    for (int i = -numLines; i <= numLines; ++i) {
        float y = i * spacing;
        vertices.push_back(glm::vec3(-size, y, 0.0f));
        vertices.push_back(glm::vec3(size, y, 0.0f));
    }

    // Generate vertical lines
    for (int i = -numLines; i <= numLines; ++i) {
        float x = i * spacing;
        vertices.push_back(glm::vec3(x, -size, 0.0f));
        vertices.push_back(glm::vec3(x, size, 0.0f));
    }
}

void Grid::updateVBO() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Grid::updateGrid(float newSize, float newSpacing) {
    size = newSize;
    spacing = newSpacing;
    generateGridVertices();
    updateVBO();
}

void Grid::draw(const glm::mat4& viewProjectionMatrix) {
    if (!isVisible) return;

    // Use a simple shader for grid rendering
    // Assume shaderProgram is already created and bound
    Scene* scene = new Scene();

    GLuint shaderProgram = scene->programs[0];
    glUseProgram(shaderProgram);
    Grid grid(100.0f, 5.0f, glm::vec3(0.5f, 0.5f, 0.5f), shaderProgram);


    // Set the color uniform
    GLint colorLocation = glGetUniformLocation(shaderProgram, "gridColor");
    glUniform3fv(colorLocation, 1, &color[0]);

    // Set the view-projection matrix uniform
    GLint vpLocation = glGetUniformLocation(shaderProgram, "viewProjectionMatrix");
    glUniformMatrix4fv(vpLocation, 1, GL_FALSE, &viewProjectionMatrix[0][0]);

    // Bind the VAO and draw the grid
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, vertices.size());

    // Unbind the VAO
    glBindVertexArray(0);
}

void Grid::toggleVisibility() {
    isVisible = !isVisible;
}

void Grid::setColor(const glm::vec3& newColor) {
    color = newColor;
}

void Grid::setSize(float newSize) {
    size = newSize;
    generateGridVertices();
    updateVBO();
}

void Grid::setSpacing(float newSpacing) {
    spacing = newSpacing;
    generateGridVertices();
    updateVBO();
}