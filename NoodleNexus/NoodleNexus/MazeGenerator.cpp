#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians


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
<<<<<<< HEAD
                PlaceModelOnGrid("assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, CENTER, true);
                PlaceModelOnGrid("assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, CENTERup, true);

                if (row > 0 && maze[row - 1][col] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, DOWN, true);
                }
                if (row < maze.size() - 1 && maze[row + 1][col] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, UP, true);
                }
                if (col > 0 && maze[row][col - 1] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, LEFT, true);
                }
                if (col < maze[row].size() - 1 && maze[row][col + 1] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, RIGHT, true);
=======
                PlaceModelOnGrid("assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply", row, col, 1.0f, CENTER, true);
                PlaceModelOnGrid("assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply", row, col, 1.0f, CENTERup, true);

                if (row > 0 && maze[row - 1][col] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f, DOWN, true);
                }
                if (row < maze.size() - 1 && maze[row + 1][col] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f, UP, true);
                }
                if (col > 0 && maze[row][col - 1] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f, LEFT, true);
                }
                if (col < maze[row].size() - 1 && maze[row][col + 1] == '0') {
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f, RIGHT, true);
>>>>>>> dc70541c9f8b90ee7f932d8ce8153801c99950fe
                }
            }
        }
    }
}


// General function to place a model on the grid
void MazeGenerator::PlaceModelOnGrid(std::string path, int row, int col, float scale, Direction type, bool isVisible) {
<<<<<<< HEAD
    // Use scale to adjust both position and size
    glm::vec3 position(col * scale * 5.0f, 0.0f, row * scale * 5.0f); // Scaling spacing
=======
    glm::vec3 position(col * scale * 5.0f, 0.0f, row * scale * 5.0f);
>>>>>>> dc70541c9f8b90ee7f932d8ce8153801c99950fe
    glm::vec3 rotation(0.0f);
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);
    

    switch (type) {
    case CENTER:
        position.z -= 2.5f * scale;
        position.x += 2.5f * scale;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        break;
    case CENTERup:
        position.y += scale * 5.0f;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case RIGHT:
        position.x += scale * 5.0f / 2.0f;
        rotation.y = -90.0f;
        color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        break;
    case LEFT:
        position.x -= scale * 5.0f / 2.0f;
        rotation.y = -270.0f;
        color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        break;
    case UP:
        position.z += scale * 5.0f / 2.0f;
        rotation.y = 180.0f;
        break;
    case DOWN:
        position.z -= scale * 5.0f / 2.0f;
        
        break;
    default:
        std::cerr << "Unknown ObjectType." << std::endl;
        return;
    }

    // Generate the object with the applied scale to actually affect the mesh size
    Object* obj = scene->GenerateMeshObjectsFromObject(
        path, position, scale, rotation, false, color, false, scene->sceneObjects);

    if (obj == nullptr) {
        std::cerr << "Failed to create object for type: " << type << std::endl;
        return;
    }

    obj->isTemporary = true;

    // Set visibility for invisible walls
    if (!isVisible) {
        obj->mesh->bIsVisible = false;
    }

    // Add to physics manager with the applied scale
    scene->physicsManager->AddTriangleMesh(path, position, rotation, scale);
<<<<<<< HEAD
}
=======
    //rotation.y += 180.f;
    //scene->physicsManager->AddTriangleMesh(path, position, rotation, scale);

}
>>>>>>> dc70541c9f8b90ee7f932d8ce8153801c99950fe
