#pragma once

#include "Scene.hpp"
#include <string>
#include <vector>

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene);
    void generateMaze();

private:
    std::vector<std::vector<char>> maze;
    Scene* scene = nullptr;

    void loadMaze(const std::string& filePath);
    void placeFloorAndCeiling(int row, int col);
    void placeWall(int row, int col);
    void placeWallRotated(int row, int col);
};
