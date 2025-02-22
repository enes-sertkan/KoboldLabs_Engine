#include "EndlessGridDemo.h"
#include <iostream>

EndlessGridDemo::EndlessGridDemo() : BaseGLApp(1920, 1080, "The Endless Grid") {
    m_cameraPos = glm::vec3(0.0f, 0.75f, -2.5f);
    m_cameraFront = glm::vec3(0.0f, -0.2f, 1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    m_cameraSpeed = 0.1f;
}

void EndlessGridDemo::Start() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(0.5, 0.5, 0.5, 1.0);
        }
    )";

    m_shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
    CreateGrid();
    Run();
}

void EndlessGridDemo::OnFrame(double deltaTime) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindVertexArray(m_vao);
    glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_INT, 0);
}

GLuint EndlessGridDemo::CreateShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void EndlessGridDemo::CreateGrid() {
    const int gridSize = 10;
    const float cellSize = 0.5f;

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;

    for (int i = -gridSize; i <= gridSize; ++i) {
        vertices.push_back(glm::vec3(i * cellSize, 0.0f, -gridSize * cellSize));
        vertices.push_back(glm::vec3(i * cellSize, 0.0f, gridSize * cellSize));
        vertices.push_back(glm::vec3(-gridSize * cellSize, 0.0f, i * cellSize));
        vertices.push_back(glm::vec3(gridSize * cellSize, 0.0f, i * cellSize));
    }

    for (int i = 0; i < vertices.size(); ++i) {
        indices.push_back(i);
    }

    m_indexCount = indices.size();

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}