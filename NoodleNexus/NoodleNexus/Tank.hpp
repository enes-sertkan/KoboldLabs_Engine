#pragma once

#include "sObject.h"
#include "Scene.hpp"
#include <glm/glm.hpp>

const int MAZE_ROWS = 61;
const int MAZE_COLS = 102;
extern int maze[MAZE_ROWS][MAZE_COLS];

class Tank {
public:

    Tank(Scene* scene, glm::vec3 position, glm::vec4 color);
    // Tank attributes
    Object* object;        // The visual representation of the tank
    sMesh* mesh;
    // Constructor to initialize mesh
    Tank(sMesh* meshData) : mesh(meshData) {}
    glm::vec3 position;     // Current position in the maze
    glm::vec3 rotation;     // Tank's orientation
    glm::vec4 color;        // Tank's color

    // Update tank's position and orientation
    void Move(int nextRow, int nextCol, int maze[MAZE_ROWS][MAZE_COLS]);

    // Check if movement is valid
    bool CheckCollision(int nextRow, int nextCol, int maze[MAZE_ROWS][MAZE_COLS]);


private:
    Scene* scene; // Reference to the current scene
};
