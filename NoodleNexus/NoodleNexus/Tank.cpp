#include "Tank.hpp"
#include <iostream>

int maze[MAZE_ROWS][MAZE_COLS] = {};


Object* object = new Object();
std::vector<Object*> sceneObjects;


// Constructor
Tank::Tank(Scene* scene, glm::vec3 position, glm::vec4 color)
    : scene(scene), position(position), rotation(0.0f), color(color) {
    object = new Object(); // Initialize object
    object->mesh = new sMesh(); // Initialize mesh
    object->mesh->positionXYZ = position;
    object->mesh->rotationEulerXYZ = rotation;
    object->mesh->objectColourRGBA = color;

    // Add the tank to the scene
    scene->sceneObjects.push_back(object);
}


// Check if the tank's next move is valid
bool Tank::CheckCollision(int nextRow, int nextCol, int maze[MAZE_ROWS][MAZE_COLS]) {
    if (nextRow < 0 || nextRow >= MAZE_ROWS || nextCol < 0 || nextCol >= MAZE_COLS) {
        return false; // Out of bounds
    }
    if (maze[nextRow][nextCol] == 1) {
        return false; // Wall detected
    }
    return true; // Movement is valid
}

// Move the tank based on input
void Tank::Move(int nextRow, int nextCol, int maze[MAZE_ROWS][MAZE_COLS]) {
    if (CheckCollision(nextRow, nextCol, maze)) {
        // Update tank position
        position.x = nextRow;
        position.z = nextCol;

        // Update mesh's position
        object->mesh->positionXYZ  = position;
    }
    else {
        std::cout << "Collision detected at (" << nextRow << ", " << nextCol << ")" << std::endl;
    }
}
