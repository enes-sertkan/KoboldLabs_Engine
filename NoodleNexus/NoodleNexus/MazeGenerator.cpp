#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians


// Constructor
MazeGenerator::MazeGenerator(const std::string& filePath, Scene* scene, cLightManager* lightManager) : scene(scene) {
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
            if (c == '0' || c == '1' || c == '2' || c == '3') {
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

            if (cell == '1' || cell == '2' || cell == '3') {
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
                }

                //// Place light only for cells marked '2', without a model
                //if (cell == '2') {
                //    PlaceLight(row, col);  // Only add the light source without a visual model
                //}

                // Determine door placement and orientation
                if (cell == '3') {
                    Direction doorDirection;
                    if (col > 0 && maze[row][col - 1] == '1') {  // Left neighbor is a wall
                        doorDirection = DOORleft;
                    }
                    else if (col < maze[row].size() - 1 && maze[row][col + 1] == '1') {  // Right neighbor is a wall
                        doorDirection = DOORright;
                    }
                    else if (row > 0 && maze[row - 1][col] == '1') {  // Top neighbor is a wall
                        doorDirection = DOORup;
                    }
                    else if (row < maze.size() - 1 && maze[row + 1][col] == '1') {  // Bottom neighbor is a wall
                        doorDirection = DOORdown;
                    }
                    else {
                        continue;  // Skip door placement if no wall is adjacent
                    }
                    // Place the door with the determined orientation
                    PlaceModelOnGrid("assets/models/Ply/SM_Env_Door_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, doorDirection, true);
                }

            }
        }
    }
}



// General function to place a model on the grid
void MazeGenerator::PlaceModelOnGrid(std::string path, int row, int col, float scale, Direction type, bool isVisible) {
    // Use scale to adjust both position and size
    glm::vec3 position(col * scale * 5.0f, 0.0f, row * scale * 5.0f); // Scaling spacing
    glm::vec3 rotation(0.0f);
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);
    glm::vec3 objectScale(1, 1, 1);


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
    case DOORleft:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        position.x -= scale * 0.5f;
        rotation.y = 0.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORright:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        position.x -= scale * -2.5f;
        rotation.y = 180.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORup:
        position.z += scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        rotation.y = 90.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORdown:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        rotation.y = -90.0f;
        scale = 7.0f * 1.6f;
        break;
    default:
        std::cerr << "Unknown ObjectType." << std::endl;
        return;
    }

    // Generate the object with the applied scale to actually affect the mesh size
    Object* obj = scene->GenerateMeshObjectsFromObject(
        path, position, scale * objectScale.x, rotation, false, color, true, scene->sceneObjects);


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
}

//void MazeGenerator::PlaceLight(int row, int col) {
//    glm::vec4 position(col * 7.0f * 5.0f, 10.0f, row * 7.0f * 5.0f, 1.0f);  // Position the light above the floor level
//    glm::vec4 diffuse(1.0f, 0.9f, 1.0f, 1.0f);  // Light color (warm white)
//    glm::vec3 attenuation(0.0f, 0.1f, 0.01f);  // Set attenuation factors
//    glm::vec4 direction(-4.48f, -9.19999f, 4.08f, 1.0f);  // Downward direction if spotlight
//    glm::vec3 param1(1.0f, 20.0f, 40.0f);  // Parameter values for light type
//    float param2x = 1.0f;  // Turn on the light
//
//    scene->SetLightFromObject(lightManager, lightIndex++, position, diffuse, attenuation, direction, param1, param2x);
//}

