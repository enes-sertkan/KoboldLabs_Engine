// MazeGenerator.hpp
#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Scene.hpp"

// Enum for different object types
enum ObjectType {
    WallRight,
    WallLeft,
    WallTop,
    WallBottom,
    Ceiling,
    Floor
};

enum Direction {
    Right,
    Left,
    Up,
    Down
};

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene);
    void generateMaze();

private:
    void loadMaze(const std::string& filePath);
    void placeWallRight(int row, int col, float scale);
    void placeWallLeft(int row, int col, float scale);
    void placeWallTop(int row, int col, float scale);
    void placeWallBottom(int row, int col, float scale);
    void placeFloorAndCeiling(int row, int col, float scale);

    // General function to create objects with optional invisibility
    void createObject(glm::vec3 position, glm::vec3 rotation, float scale, glm::vec4 color, ObjectType type, bool invisible = false);

    std::vector<std::vector<char>> maze;
    Scene* scene = nullptr;
};

#endif // MAZEGENERATOR_HPP
