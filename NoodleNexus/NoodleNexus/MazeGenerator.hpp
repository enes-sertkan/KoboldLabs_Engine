#ifndef MAZE_GENERATOR_HPP
#define MAZE_GENERATOR_HPP

#include "Scene.hpp"
#include "Tank.hpp" // Include Tank class
#include <string>
#include <vector>
#include <glm/glm.hpp>

enum MazeElement {
    Ground, // Empty space
    Wall,   // X
    Player, // P
    Enemy   // T
};

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene);

    void loadMaze(const std::string& filePath);
    void generateMaze();
    void createPlayerTank(int row, int col, float scale);
    Tank* getPlayerTank() const;
    std::vector<std::vector<char>> maze;


private:
    Scene* scene;
    Tank* playerTank = nullptr; // Ensure this is declared here

    void placeElementOnGrid(const std::string& modelPath, int row, int col, float scale, MazeElement type);
};

#endif // MAZE_GENERATOR_HPP
