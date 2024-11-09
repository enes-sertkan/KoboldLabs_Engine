// MazeGenerator.cpp
#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians

// Define a cell size constant to manage object spacing
const float CELL_SIZE = 5.0f;

// Constructor
MazeGenerator::MazeGenerator(const std::string& filePath, Scene* scene) : scene(scene) {
    loadMaze(filePath);
}

// Load maze from file
void MazeGenerator::loadMaze(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open maze file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Remove any whitespace or non-relevant characters if necessary
        std::vector<char> row;
        for (char c : line) {
            if (c == '0' || c == '1') {
                row.push_back(c);
            }
        }
        maze.push_back(row);
    }
    file.close();
}

// Generate maze based on 0s and 1s
void MazeGenerator::generateMaze() {
    for (size_t row = 0; row < maze.size(); ++row) {
        for (size_t col = 0; col < maze[row].size(); ++col) {
            char cell = maze[row][col];

            if (cell == '1') {
                // Place floor and ceiling for walkable cells
                placeFloorAndCeiling(static_cast<int>(row), static_cast<int>(col), 1.0f);

                // Place walls based on neighboring cells
                // Top wall
                if (row > 0 && maze[row - 1][col] == '0') {
                    placeWallBottom(static_cast<int>(row), static_cast<int>(col), 1.0f);
                }
                // Bottom wall
                if (row < maze.size() - 1 && maze[row + 1][col] == '0') {
                    placeWallTop(static_cast<int>(row), static_cast<int>(col), 1.0f);
                }
                // Left wall
                if (col > 0 && maze[row][col - 1] == '0') {
                    placeWallLeft(static_cast<int>(row), static_cast<int>(col), 1.0f);
                }
                // Right wall
                if (col < maze[row].size() - 1 && maze[row][col + 1] == '0') {
                    placeWallRight(static_cast<int>(row), static_cast<int>(col), 1.0f);
                }
            }
        }
    }
}

// Place floor and ceiling for each walkable cell
void MazeGenerator::placeFloorAndCeiling(int row, int col, float scale) {
    glm::vec3 floorPosition(col * CELL_SIZE, 0, row * CELL_SIZE);
    glm::vec4 floorColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Place floor
    floorPosition.z -= 2.5f;
    floorPosition.x += 2.5f;

    createObject(floorPosition, glm::vec3(0.0f), scale, floorColor, Floor, false);

    // Place ceiling above floor
    glm::vec3 ceilingPosition = floorPosition;
    ceilingPosition.y += CELL_SIZE; // Adjust height for ceiling placement
    createObject(ceilingPosition, glm::vec3(0.0f), scale, floorColor, Ceiling, true);
}

// Place a wall on the right side for vertical alignment
void MazeGenerator::placeWallRight(int row, int col, float scale) {
    glm::vec3 position(col * CELL_SIZE + (CELL_SIZE / 2.0f), 0.0f, row * CELL_SIZE); // Position for right wall
    glm::vec3 rotation(0.0f, 90.0f, 0.0f); // Rotate wall to face right
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);

    // Place visible wall on the right
    createObject(position, rotation, scale, color, WallRight, false);

    // Place invisible wall rotated 180 degrees
    glm::vec3 rotation_invisible = rotation;
    rotation_invisible.y += 180.0f;
    createObject(position, rotation_invisible, scale, color, WallLeft, true);
}

// Place a wall on the left side for vertical alignment
void MazeGenerator::placeWallLeft(int row, int col, float scale) {
    glm::vec3 position(col * CELL_SIZE - (CELL_SIZE / 2.0f), 0.0f, row * CELL_SIZE); // Position for left wall
    glm::vec3 rotation(0.0f, 270.0f, 0.0f); // Rotate wall to face left
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);

    // Place visible wall on the left
    createObject(position, rotation, scale, color, WallLeft, false);

    // Place invisible wall rotated 180 degrees
    glm::vec3 rotation_invisible = rotation;
    rotation_invisible.y += 180.0f;
    createObject(position, rotation_invisible, scale, color, WallRight, true);
}

// Place a wall on the top side for horizontal alignment
void MazeGenerator::placeWallTop(int row, int col, float scale) {
    glm::vec3 position(col * CELL_SIZE, 0.0f, row * CELL_SIZE + (CELL_SIZE / 2.0f)); // Position for top wall
    glm::vec3 rotation(0.0f, 0.0f, 0.0f); // No rotation needed for top
    glm::vec4 color(0.3f, 0.3f, 0.3f, 1.0f);

    // Place visible wall on the top
    createObject(position, rotation, scale, color, WallTop, false);

    // Place invisible wall rotated 180 degrees
    glm::vec3 rotation_invisible = rotation;
    rotation_invisible.y += 180.0f;
    createObject(position, rotation_invisible, scale, color, WallBottom, true);
}

// Place a wall on the bottom side for horizontal alignment
void MazeGenerator::placeWallBottom(int row, int col, float scale) {
    glm::vec3 position(col * CELL_SIZE, 0.0f, row * CELL_SIZE - (CELL_SIZE / 2.0f)); // Position for bottom wall
    glm::vec3 rotation(0.0f, 180.0f, 0.0f); // Rotate wall to face bottom
    glm::vec4 color(0.3f, 0.3f, 0.3f, 1.0f);

    // Place visible wall on the bottom
    createObject(position, rotation, scale, color, WallBottom, false);

    // Place invisible wall rotated 180 degrees
    glm::vec3 rotation_invisible = rotation;
    rotation_invisible.y += 180.0f;
    createObject(position, rotation_invisible, scale, color, WallTop, true);
}

// General function to create objects with optional invisibility
void MazeGenerator::createObject(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 color, ObjectType type, bool invisible) {
    const char* assetPath = nullptr;
    std::string tag;

    // Determine the asset path and tag based on type
    switch (type) {
    case WallRight:
        assetPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
        tag = "wall_right";
        break;
    case WallLeft:
        assetPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
        tag = "wall_left";
        break;
    case WallTop:
        assetPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
        tag = "wall_top";
        break;
    case WallBottom:
        assetPath = "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply";
        tag = "wall_bottom";
        break;
    case Ceiling:
        assetPath = "assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply";
        tag = "ceiling";
        break;
    case Floor:
        assetPath = "assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply";
        tag = "floor";
        break;
    default:
        std::cerr << "Unknown ObjectType. Setting default assetPath." << std::endl;
        assetPath = "assets/models/Ply/Default_Asset.ply";
        tag = "unknown";
        break;
    }

    // Ensure assetPath is set
    if (assetPath == nullptr) {
        std::cerr << "Asset path not set for ObjectType: " << tag << std::endl;
        return;
    }

    // Generate the object
    Object* obj = scene->GenerateMeshObjectsFromObject(
        assetPath, position, scale, rotation, false, color, false, scene->sceneObjects);

    if (obj == nullptr) {
        std::cerr << "Failed to create object for tag: " << tag << std::endl;
        return;
    }

    obj->isTemporary = true;
    obj->tags.push_back(tag);

    // Set visibility for invisible walls
    if (invisible) {
        obj->mesh->bIsVisible = false;
    }

    // Add to physics manager
    scene->physicsManager->AddTriangleMesh(assetPath, position, rotation, scale);
}
