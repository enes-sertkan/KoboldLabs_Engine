// MazeGenerator.hpp

#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Scene.hpp"
#include "cLightManager.h" // Include the light manager

// Enum for different object types
enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    CENTER,
    CENTERup,
    LIGHT,
    DOORright,
    DOORleft,
    DOORup,
    DOORdown
};

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene, cLightManager* lightManager);  // Add lightManager
    void generateMaze();

private:
    void loadMaze(const std::string& filePath);
    void PlaceModelOnGrid(std::string path, int row, int col, float scale, Direction direction, bool invisible = false, glm::vec4 color = glm::vec4(0.5,0.5,0.5,1.f));
    //void PlaceLight(int row, int col);  // New method to place a light

    std::vector<std::vector<char>> maze;
    Scene* scene = nullptr;
    int lightIndex = 0;  // Index to keep track of lights
};

#endif // MAZEGENERATOR_HPP
