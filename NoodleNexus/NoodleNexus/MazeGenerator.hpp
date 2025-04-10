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
class SoftBody;

class LabAttackFactory;
// Enum for different object types
enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN,
    CENTER,
    CENTERFLOOR,
    CENTERup,
    LIGHT,
    VENTS,
    TUBES,
    REACTORS,
    TABLE,
    BROKENTUBES,
    SOFTCENTER,
    BRUTEENEM
};


class MazeGenerator {
public:
    float floorHeight = 3.f;
    const float TILE_SIZE = 1.0f * 4.0f;
    SoftBody* mainSlime = nullptr;
    MazeGenerator(const std::string& filePath, Scene* scene, cLightManager* lightManager);  // Add lightManager
    void generateMaze();
    int GetRows();
    int GetCols();
    bool IsWall(int x, int y);
    bool IsFloor(int x, int y);
    glm::vec3 GridToWorld(int x, int y) ;
    char GetMazePoint(int x, int y);

    //void PlaceFood(int count = 50);
    std::vector<Object*> foods;

    //void PlaceWater(int count = 100);
    std::vector<Object*> waters;

    std::vector<glm::vec2> controlPoints;

    Object* player = nullptr;

    LabAttackFactory* factory = nullptr;

    glm::vec2 WorldToGrid(const glm::vec3& worldPos) const {
       // Match GridToWorld scaling
        int gridCol = static_cast<int>(worldPos.x / TILE_SIZE);
        int gridRow = static_cast<int>(worldPos.z / TILE_SIZE);
        return glm::vec2(gridCol, gridRow);
    }

private:

    std::vector<std::vector<char>> maze;
    void loadMaze(const std::string& filePath);
    Object* PlaceModelOnGrid(std::string path, int row, int col, int floor, float scale, Direction direction, bool invisible = false, glm::vec4 color = glm::vec4(0.5,0.5,0.5,1.f), Object* parent = nullptr);
    void PlaceRandomObjects();

    std::vector<std::vector<bool>> occupiedPositions;
    bool IsPositionOccupied(int row, int col) ;
    void MarkPositionOccupied(int row, int col);
    //void PlaceRandomSizedObject(int row, int col, Direction type)  // New method to place a light



    Scene* scene = nullptr;
    int lightIndex = 0;  // Index to keep track of lights

   

};
