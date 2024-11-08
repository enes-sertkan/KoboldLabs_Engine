#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"

MazeGenerator::MazeGenerator(const std::string& filePath, Scene* scene) : scene(scene) {
    loadMaze(filePath);
}

void MazeGenerator::loadMaze(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open maze file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row(line.begin(), line.end());
        maze.push_back(row);
    }
    file.close();
}

void MazeGenerator::generateMaze() {
    for (size_t row = 0; row < maze.size(); ++row) {
        for (size_t col = 0; col < maze[row].size(); ++col) {
            char cell = maze[row][col];

            if (cell != '1') {
                // Place floor and ceiling for all non-walkable cells
                placeFloorAndCeiling(static_cast<int>(row)* 2, static_cast<int>(col) * 2);
            }

            if (cell == '2') {
                // Place a wall with no rotation
                placeWall(static_cast<int>(row) * 2, static_cast<int>(col) * 2);
            }
            else if (cell == '0') {
                // Place a wall rotated 90 degrees on the Y-axis
                placeWallRotated(static_cast<int>(row) * 2, static_cast<int>(col) * 2);
            }
        }
    }
}

void MazeGenerator::placeFloorAndCeiling(int row, int col) {
    glm::vec3 position(col, 0, row);
    glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    float scale = 1.0f;

    // Create floor
    position.x += 5.f;
    Object* floor = scene->GenerateMeshObjectsFromObject(
        "assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply", position, scale, rotation,
        false, color, false, scene->sceneObjects);
    floor->isTemporary = true;
    floor->tags.push_back("floor");
    scene->physicsManager->AddTriangleMesh("assets/models/Ply/SM_Env_Floor_01_xyz_n_rgba_uv.ply", position, rotation, scale);

    // Create ceiling with an offset in height
    position.y += 5.f;
    position.z += 5.f;
    Object* ceiling = scene->GenerateMeshObjectsFromObject(
        "assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply", position, scale, rotation,
        false, color, false, scene->sceneObjects);
    ceiling->isTemporary = true;
    ceiling->mesh->bIsVisible = false;
    ceiling->tags.push_back("ceiling");
}

void MazeGenerator::placeWall(int row, int col) {
    glm::vec3 position(col, 0.f, row); 
    glm::vec3 rotation(0.0f, 0.0f, 0.0f); 
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);
    float scale = 1.0f;

    // Create the wall object
    position.x += 3.f;
    position.z += 2.5f;
    Object* wall = scene->GenerateMeshObjectsFromObject(
        "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", position, scale, rotation,
        false, color, false, scene->sceneObjects);
    wall->isTemporary = true;
    wall->tags.push_back("wall");
}

void MazeGenerator::placeWallRotated(int row, int col) {
    glm::vec3 position(col, 0.f, row); 
    glm::vec3 rotation(0.0f, 90.0f, 0.0f); 
    glm::vec4 color(0.3f, 0.3f, 0.3f, 1.0f);
    float scale = 1.0f;

    position.x += 2.0f;

    //TODO: Pl, so GenerateMeshObjectsFromObject doesn't have scale as parametr. Add ita
    Object* wall = scene->GenerateMeshObjectsFromObject(
        "assets/models/Ply/SM_Env_Wall_02_xyz_n_rgba_uv.ply", position, scale, rotation,
        false, color, false, scene->sceneObjects);
    wall->isTemporary = true;
    wall->tags.push_back("rotatedWall");
}


