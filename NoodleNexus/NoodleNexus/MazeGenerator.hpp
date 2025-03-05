// MazeGenerator.hpp
#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "Scene.hpp"
#include "cLightManager.h" // Include the light manager

class BazeMazeCharacter;
class MinotaurChar;

// Enum for different object types
enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    CENTER,
    CENTERup,
    LIGHT,
    MINOTAUR,
    THESEUS,
    WATER,
    FOOD
};

class MazeGenerator {
public:
    MazeGenerator(const std::string& filePath, Scene* scene, cLightManager* lightManager);  // Add lightManager
    void generateMaze();

    bool IsWall(int x, int y) const;
    glm::vec3 GridToWorld(int x, int y) const;
    char GetMazePoint(int x, int y);

    void PlaceFood(int count = 50);
    std::vector<Object*> foods;

    void PlaceWater(int count = 100);
    std::vector<Object*> waters;


    BazeMazeCharacter* minoChar = nullptr;
    BazeMazeCharacter* thesChar = nullptr;

    glm::vec2 WorldToGrid(const glm::vec3& worldPos) const {
        const float TILE_SIZE = 1.0f * 7.0f * 5.0f; // Match GridToWorld scaling
        int gridCol = static_cast<int>(worldPos.x / TILE_SIZE);
        int gridRow = static_cast<int>(worldPos.z / TILE_SIZE);
        return glm::vec2(gridCol, gridRow);
    }
private:
    void loadMaze(const std::string& filePath);
    Object* PlaceModelOnGrid(std::string path, int row, int col, int floor, float scale, Direction direction, bool invisible = false, glm::vec4 color = glm::vec4(0.5,0.5,0.5,1.f));
    void PlaceRandomObjects();

    std::vector<std::vector<bool>> occupiedPositions;
    bool IsPositionOccupied(int row, int col) const;
    void MarkPositionOccupied(int row, int col);
    //void PlaceRandomSizedObject(int row, int col, Direction type)  // New method to place a light

    std::vector<std::vector<char>> maze;

    Scene* scene = nullptr;
    int lightIndex = 0;  // Index to keep track of lights

   

};
