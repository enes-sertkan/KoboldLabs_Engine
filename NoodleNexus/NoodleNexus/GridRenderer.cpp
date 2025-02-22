//#include "GridRenderer.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//GridRenderer::GridRenderer() : shaderProgram(0), vao(0), vbo(0), ebo(0) {}
//
//GridRenderer::~GridRenderer() {
//    if (vao) glDeleteVertexArrays(1, &vao);
//    if (vbo) glDeleteBuffers(1, &vbo);
//    if (ebo) glDeleteBuffers(1, &ebo);
//    if (shaderProgram) glDeleteProgram(shaderProgram);
//}
//
//bool GridRenderer::Initialize() {
//    shaderProgram = LoadShaders("infinite_grid.vs", "infinite_grid.frag");
//    if (shaderProgram == 0) {
//        std::cerr << "Failed to load grid shaders!" << std::endl;
//        return false;
//    }
//
//    SetupGridVAO();
//    return true;
//}
//
//void GridRenderer::Render(const glm::mat4& viewProjectionMatrix, const glm::vec3& cameraPosition) {
//    glUseProgram(shaderProgram);
//
//    GLuint vpUniform = glGetUniformLocation(shaderProgram, "gVP");
//    glUniformMatrix4fv(vpUniform, 1, GL_FALSE, &viewProjectionMatrix[0][0]);
//
//    GLuint cameraPosUniform = glGetUniformLocation(shaderProgram, "gCameraWorldPos");
//    glUniform3f(cameraPosUniform, cameraPosition.x, cameraPosition.y, cameraPosition.z);
//
//    GLuint gridSizeUniform = glGetUniformLocation(shaderProgram, "gGridSize");
//    glUniform1f(gridSizeUniform, 100.0f);
//
//    GLuint gridCellSizeUniform = glGetUniformLocation(shaderProgram, "gGridCellSize");
//    glUniform1f(gridCellSizeUniform, 0.025f);
//
//    glBindVertexArray(vao);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}
//
//// Rest of the implementation remains the same...
//
//GLuint GridRenderer::LoadShaders(const char* vertexFilePath, const char* fragmentFilePath) {
//    // Read vertex shader code
//    std::string vertexCode;
//    std::ifstream vertexStream(vertexFilePath, std::ios::in);
//    if (vertexStream.is_open()) {
//        std::stringstream sstr;
//        sstr << vertexStream.rdbuf();
//        vertexCode = sstr.str();
//        vertexStream.close();
//    }
//    else {
//        std::cerr << "Failed to open vertex shader file: " << vertexFilePath << std::endl;
//        return 0;
//    }
//
//    // Read fragment shader code
//    std::string fragmentCode;
//    std::ifstream fragmentStream(fragmentFilePath, std::ios::in);
//    if (fragmentStream.is_open()) {
//        std::stringstream sstr;
//        sstr << fragmentStream.rdbuf();
//        fragmentCode = sstr.str();
//        fragmentStream.close();
//    }
//    else {
//        std::cerr << "Failed to open fragment shader file: " << fragmentFilePath << std::endl;
//        return 0;
//    }
//
//    // Compile vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    const char* vertexSource = vertexCode.c_str();
//    glShaderSource(vertexShader, 1, &vertexSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Check vertex shader compilation
//    GLint success;
//    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
//        return 0;
//    }
//
//    // Compile fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    const char* fragmentSource = fragmentCode.c_str();
//    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Check fragment shader compilation
//    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
//        return 0;
//    }
//
//    // Link shaders
//    GLuint program = glCreateProgram();
//    glAttachShader(program, vertexShader);
//    glAttachShader(program, fragmentShader);
//    glLinkProgram(program);
//
//    // Check linking
//    glGetProgramiv(program, GL_LINK_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetProgramInfoLog(program, 512, NULL, infoLog);
//        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
//        return 0;
//    }
//
//    // Clean up shaders
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return program;
//}
//
//void GridRenderer::SetupGridVAO() {
//    // Vertex data for the grid (a single quad covering the entire screen)
//    const float vertices[] = {
//        -1.0f, 0.0f, -1.0f,
//         1.0f, 0.0f, -1.0f,
//         1.0f, 0.0f,  1.0f,
//        -1.0f, 0.0f,  1.0f
//    };
//
//    // Indices for the quad
//    const unsigned int indices[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    // Generate and bind VAO
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    // Generate and bind VBO
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    // Set up vertex attribute pointers
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Generate and bind EBO
//    glGenBuffers(1, &ebo);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//    // Unbind VAO
//    glBindVertexArray(0);
//}