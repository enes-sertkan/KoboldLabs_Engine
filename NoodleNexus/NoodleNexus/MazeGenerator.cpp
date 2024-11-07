#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>

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
            if (maze[row][col] == '1') {
                placeFloorAndCeiling(static_cast<int>(row), static_cast<int>(col));
            }
            else if (maze[row][col] == '0') {
                placeWall(static_cast<int>(row), static_cast<int>(col));
            }
        }
    }
}

void MazeGenerator::placeFloorAndCeiling(int row, int col) {
    glm::vec3 position(col, 0, row);
    glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);

    // Create floor
    scene->GenerateMeshObjectsFromObject("assets/models/Ply/SM_Env_Floor_03_xyz_n_rgba_uv.ply", position, rotation, false, color, true, scene->sceneObjects);

    // Create ceiling with offset
    position.y += 5.0f;
    scene->GenerateMeshObjectsFromObject("assets/models/Ply/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply", position, rotation, false, color, true, scene->sceneObjects);

}

void MazeGenerator::placeWall(int row, int col) {
    glm::vec3 position(col, 2.5f, row);  // Adjust height as needed
    glm::vec3 rotation(0.0f, 0.0f, 0.0f);
    glm::vec4 color(0.5f, 0.5f, 0.5f, 1.0f);

    // Create wall
    scene->GenerateMeshObjectsFromObject("assets/models/Ply/SM_Env_ControlRoom_Wall_01_xyz_n_rgba_uv.ply", position, rotation, false, color, true, scene->sceneObjects);
}


//MazeGenerator* mazeGenerator = new MazeGenerator("assets/models/maze.txt", scene);
//
//mazeGenerator->generateMaze();