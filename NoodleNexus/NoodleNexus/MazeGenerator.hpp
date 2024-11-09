#ifndef MAZEGENERATOR_HPP
#define MAZEGENERATOR_HPP

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Scene.hpp"

// Enum for different object types
enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    CENTER,
    CENTERup
};

enum Center {
    FLOOR,
    CEILING
};

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene);
    void generateMaze();

private:
    void loadMaze(const std::string& filePath);
    void PlaceModelOnGrid(std::string path, int row, int col, float scale, Direction direction, bool invisible = false);

    std::vector<std::vector<char>> maze;
    Scene* scene = nullptr;
};

#endif // MAZEGENERATOR_HPP
