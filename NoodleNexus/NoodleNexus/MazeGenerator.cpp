#include "MazeGenerator.hpp"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include "sObject.h"
#include "Tank.hpp" // Include Tank class
#include "PhysicsManager.h"

// Constructor
MazeGenerator::MazeGenerator(const std::string& filePath, Scene* scene)
    : scene(scene), playerTank(nullptr) { // Initialize playerTank to nullptr
    loadMaze(filePath);
}

// Load maze layout from file
void MazeGenerator::loadMaze(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open maze file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            if (c == 'P' || c == 'T' || c == 'X' || c == ' ') {
                row.push_back(c);
            }
        }
        maze.push_back(row);
    }
    file.close();
}

// Generate maze objects
void MazeGenerator::generateMaze() {
    for (size_t row = 0; row < maze.size(); ++row) {
        for (size_t col = 0; col < maze[row].size(); ++col) {
            char cell = maze[row][col];
            float scale = 10.0f;

            switch (cell) {
            case 'P': // Player
                createPlayerTank(row, col, scale);
                break;
            case 'T': // Enemy
                placeElementOnGrid("assets/models/tank_xyz_n_uv.ply", row, col, scale, Enemy);
                break;
            case 'X': // Wall
                placeElementOnGrid("assets/models/Cube_xyz_n_uv.ply", row, col, scale, Wall);
                
                // Add wall to physics manager
                glm::vec3 wallPosition(col * scale, scale * 1.5f, row * scale);
                glm::vec3 wallRotation(0.0f);
                glm::vec3 wallScale(scale * 0.25f);

                scene->physicsManager->AddTriangleMesh(
                    "assets/models/Cube_xyz_n_uv.ply",
                    wallPosition,
                    wallRotation,
                    wallScale.x
                );
                break;
            case ' ': // Ground
            default:
                placeElementOnGrid("assets/models/Cube_xyz_n_uv.ply", row, col, scale, Ground);
                break;
            }
        }
    }
}

// Create the player tank
void MazeGenerator::createPlayerTank(int row, int col, float scale) {
    glm::vec3 position(col * scale, 0.0f, row * scale); // Position based on grid
    glm::vec4 color(0.0f, 1.0f, 0.0f, 1.0f);           // Green color for player tank

    // Create the Tank object
    playerTank = new Tank(scene, position, color);
    if (!playerTank) {
        std::cerr << "Failed to create player tank at row: " << row << ", col: " << col << std::endl;
        return;
    }
}

// Place an element in the grid
void MazeGenerator::placeElementOnGrid(const std::string& modelPath, int row, int col, float scale, MazeElement type) {
    glm::vec3 position(col * scale, 0.0f, row * scale); // Position based on grid
    glm::vec3 rotation(0.0f);
    glm::vec3 objectScale(1.0f);

    glm::vec4 color{}; // Assign unique colors based on element type
    switch (type) {
    case Player:
        color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f); // Green
        break;
    case Enemy:
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
        objectScale = glm::vec3(0.3f, 1.0f, 0.3f);
        position.y += scale * 1.5f;
        break;
    case Wall:
        color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray
        objectScale = glm::vec3(0.5f, 1.0f, 0.5f);
        position.y += scale * 1.5f;
        break;
    case Ground:
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // White
        break;
    }

    Object* obj = scene->GenerateMeshObjectsFromObject(
        modelPath, position, scale * objectScale.x, rotation, true, color, false, scene->sceneObjects);

    if (!obj) {
        std::cerr << "Failed to create object at row: " << row << ", col: " << col << std::endl;
        return;
    }

    obj->isTemporary = true;

    // Add object to physics manager (optional, depends on your requirements)
    scene->physicsManager->AddTriangleMesh(modelPath, position, rotation, scale);
}


Tank* MazeGenerator::getPlayerTank() const {
    return playerTank;
}

