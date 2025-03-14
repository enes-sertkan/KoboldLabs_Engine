#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians
#include <random>
#include "aMinotaur.h"
#include "Theseus.h"


// Constructor
MazeGenerator::MazeGenerator(const std::string& filePath, Scene* scene, cLightManager* lightManager) : scene(scene) {
    loadMaze(filePath);
}

// Load maze from file
void MazeGenerator::loadMaze(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open maze file: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<char> row;
        for (char c : line) {
            if (c == 'X' || c == '.' || c == 'T' || c == 'M' || c == 'B' || c == 'R' || c == 'V') {
                row.push_back(c);
            }
        }
        maze.push_back(row);
    }
    file.close();
}

void MazeGenerator::generateMaze() {
    int floor = 1;
    for (size_t row = 0; row < maze.size(); ++row) {
        for (size_t col = 0; col < maze[row].size(); ++col) {
            char cell = maze[row][col];
            occupiedPositions.resize(maze.size(), std::vector<bool>(maze[0].size(), false));

            if (cell == '.' || cell == 'M' || cell == 'T' || cell == 'R' || cell == 'B') {
                PlaceModelOnGrid("assets/models/objects/floor.ply", row, col, floor, 1.0f, CENTER, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
               // PlaceModelOnGrid("assets/models/objects/floor.ply", row, col, floor, 1.0f * 7.0f, CENTERup, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

                // Place surrounding walls
                if (row > 0 && maze[row - 1][col] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/wall01.ply", row, col, floor, 1.0f, DOWN, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f));
                    PlaceModelOnGrid("assets/models/objects/wall01.ply", row, col, floor + 1.0f, 1.0f, DOWN, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f));
                }
                if (row < maze.size() - 1 && maze[row + 1][col] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/wall01.ply", row, col, floor, 1.0f, UP, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                    PlaceModelOnGrid("assets/models/objects/wall01.ply", row, col, floor + 1.0f, 1.0f, UP, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                if (col > 0 && maze[row][col - 1] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor, 1.0f, LEFT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                    PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor + 1.0f, 1.0f, LEFT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                if (col < maze[row].size() - 1 && maze[row][col + 1] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor, 1.0f, RIGHT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                    PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor + 1.0f, 1.0f, RIGHT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }

                //// VENTS
                //if (row > 0 && maze[row - 1][col] == 'V') {
                //    PlaceModelOnGrid("assets/models/objects/vent.ply", row + 5, col, floor + 1.0f, 1.0f * 7.0f, VENTS, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f)); //VENT down

                //}
                //if (row < maze.size() - 1 && maze[row + 1][col] == 'V') {
                //    PlaceModelOnGrid("assets/models/objects/vent.ply", row + 5, col, floor + 1.0f, 1.0f * 7.0f, VENTS, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f)); //VENT up

                //}
                //if (col > 0 && maze[row][col - 1] == 'V') {
                //    PlaceModelOnGrid("assets/models/objects/vent.ply", row + 5, col, floor + 1.0f, 1.0f * 7.0f, VENTS, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f)); //VENT left

                //}
                //if (col < maze[row].size() - 1 && maze[row][col + 1] == 'V') {
                //    PlaceModelOnGrid("assets/models/objects/vent.ply", row + 5, col, floor + 1.0f, 1.0f * 7.0f, VENTS, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f)); //VENT right

                //}

                // Door placement logic (optional, as in your example)
                //if (cell == 'T') {
                //    Direction doorDirection;
                //    if (col > 0 && maze[row][col - 1] == '1') {
                //        doorDirection = DOORleft;
                //    }
                //    else if (col < maze[row].size() - 1 && maze[row][col + 1] == '1') {
                //        doorDirection = DOORright;
                //    }
                //    else if (row > 0 && maze[row - 1][col] == '1') {
                //        doorDirection = DOORup;
                //    }
                //    else if (row < maze.size() - 1 && maze[row + 1][col] == '1') {
                //        doorDirection = DOORdown;
                //    }
                //    else {
                //        continue;
                //    }
                //    PlaceModelOnGrid("assets/models/extras/SM_Env_Door_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, doorDirection, true);
                //}

                // Place random objects in '1' cells
                if (cell == 'M') {
                    PlaceModelOnGrid("", row, col, floor, 1.0f * 7.0f, TABLE, true);
                }
                else if (cell == 'T') {
                    PlaceModelOnGrid("", row, col, floor, 1.0f * 7.0f, TUBES, true);
                }
                else if (cell == 'B') {
                    PlaceModelOnGrid("", row, col, floor, 1.0f * 7.0f, BROKENTUBES, true);
                }
                else if (cell == 'R') {
                    PlaceModelOnGrid("", row, col, floor, 1.0f * 7.0f, REACTORS, true);
                }

                
            }
        }
    }
    //PlaceFood(50);
    //PlaceWater(100);
}

Object* MazeGenerator::PlaceModelOnGrid(std::string path, int row, int col, int floor, float scale, Direction type, bool isVisible, glm::vec4 color) {
    glm::vec3 position(col * scale * 4.0f, floor * scale * 3.0f, row * scale * 4.0f);
    glm::vec3 rotation(0.0f);
    glm::vec3 objectScale(1, 1, 1);
    std::string texture;
    bool isOverrideColor = false;

    switch (type) {
    //case TUBES: {
    //    std::vector<std::string> mediumObjectPaths = {
    //        "assets/models/objects/tube.ply"
    //        //"assets/models/objects/tubeGlass.ply"
    //    };
    //    path = mediumObjectPaths[rand() % mediumObjectPaths.size()];  // Medium scale for medium objects
    //    scale *= 1.f;  // Smaller scale for small objects
    //    isOverrideColor = true;
    //    color = glm::vec4(0.7, 0.6, 0.3, 1.0f);
    //    texture = "Frame_Tube_AlbedoTransparency.bmp";
    //    break;
    //}
    //case REACTORS: {
    //    std::vector<std::string> bigObjectPaths = {
    //        "assets/models/objects/reactor.ply"
    //    };
    //    path = bigObjectPaths[rand() % bigObjectPaths.size()];
    //    scale *= 1.2f;  // Larger scale for big objects
    //    color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    //    texture = "Reactor_AlbedoTransparency.bmp";
    //    break;
    //}
    //case TABLE: {
    //    std::vector<std::string> bigObjectPaths = {
    //        "assets/models/objects/table.ply",
    //    };
    //    path = bigObjectPaths[rand() % bigObjectPaths.size()];
    //    scale *= 1.2f;  // Larger scale for big objects
    //    color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    //    texture = "Operating_Table_AlbedoTransparency.bmp";
    //    break;
    //}
    //case BROKENTUBES: {
    //    std::vector<std::string> bigObjectPaths = {
    //        "assets/models/objects/tubeBroken.ply",
    //        //"assets/models/objects/shards.ply",
    //    };
    //    path = bigObjectPaths[rand() % bigObjectPaths.size()];
    //    scale *= 1.2f;  // Larger scale for big objects
    //    color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    //    texture = "Frame_Tube_AlbedoTransparency.bmp";
    //    break;
    //}
    case CENTER:
        position.z -= 2.5f * scale;
        position.x += 2.5f * scale;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        texture = "Floor_Albedo.bmp";
        break;
    case CENTERup:
        position.z += 1.5f * scale;
        position.x += 2.5f * scale;
        position.y += 6.0f * scale;
        rotation.x = 180;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        texture = "Floor_Albedo.bmp";
        break;
    case RIGHT:
        position.x += scale * 5.0f / 2.0f;
        position.z -= scale * 5.0f / 2.0f;
        color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        texture = "Wall_Simple_AlbedoTransparency.bmp";
        break;
    case LEFT:
        position.x -= scale * 3.0f / 2.0f;
        position.z -= scale * 5.0f / 2.0f;
        color = glm::vec4(0.3f, 0.6f, 0.3f, 1.0f);
        texture = "Wall_Simple_AlbedoTransparency.bmp";
        break;
    case UP:
        position.z += scale * 3.0f / 2.0f;
        position.x += scale * 5.0f / 2.0f;
        texture = "Wall_Simple_AlbedoTransparency.bmp";
        break;
    case DOWN:
        position.z -= scale * 5.0f / 2.0f;
        position.x += scale * 5.0f / 2.0f;
        texture = "Wall_Simple_AlbedoTransparency.bmp";
        break;
    //case VENTS:
    //    position.z -= scale * 5.0f / 2.0f;
    //    position.x += scale * 5.0f / 2.0f;
    //    texture = "Vent_Big_AlbedoTransparency.bmp";
    //    break;
    default:
        std::cerr << "Unknown ObjectType." << std::endl;
        return nullptr;
    }

    // Generate the object with the applied scale to actually affect the mesh size
    Object* obj = scene->GenerateMeshObjectsFromObject(
        path, position, scale * objectScale.x, rotation,false, glm::vec4(1.0, 0.2, 0.2, 1.0f), true, scene->sceneObjects);


    if (obj == nullptr) {
        std::cerr << "Failed to create object for type: " << type << std::endl;
        return nullptr;
    }

    //if (type == MINOTAUR)
    //{
    //    MinotaurChar* minotaur = new MinotaurChar();
    //    minotaur->mazePosition.x = col;
    //    minotaur->mazePosition.y = row;
    //    minotaur->maze = this;
    //    scene->AddActionToObj(minotaur, obj);
    //    minoChar = minotaur;
    //}
    //else if (type == THESEUS)
    //{
    //    TheseusChar* theseus = new TheseusChar();
    //    theseus->mazePosition.x = col;
    //    theseus->mazePosition.y = row;
    //    theseus->maze = this;
    //    scene->AddActionToObj(theseus, obj);
    //    thesChar = theseus;
    //}

    obj->isTemporary = true;
    obj->mesh->drawBothFaces = true;
    obj->mesh->textures[0] = texture;
    obj->mesh->blendRatio[0] = 1;
    obj->mesh->textureFillType[0] = 1;
    obj->mesh->objectColourRGBA = color;
    obj->mesh->bOverrideObjectColour = isOverrideColor;
    // Set visibility for invisible walls
    if (!isVisible) {
        obj->mesh->bIsVisible = false;
    }


    // Add to physics manager with the applied scale
    scene->physicsManager->AddTriangleMesh(path, position, rotation, scale);

    return obj;
}

bool MazeGenerator::IsWall(int x, int y) const {
    if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size())
        return true;
    return maze[x][y] == 'X';
}

glm::vec3 MazeGenerator::GridToWorld(int x, int y) const {
    const float TILE_SIZE = 1.0f * 7.0* 5.0f; // Match your scaling factor
    return glm::vec3(x * TILE_SIZE, 0, y * TILE_SIZE);
}

char MazeGenerator::GetMazePoint(int x, int y) {
    return maze[x][y];
}

bool MazeGenerator::IsPositionOccupied(int row, int col) const {
    if (row < 0 || col < 0 || row >= occupiedPositions.size() || col >= occupiedPositions[0].size())
        return true; // Out of bounds is considered occupied
    return occupiedPositions[row][col];
}

// Mark a position as occupied
void MazeGenerator::MarkPositionOccupied(int row, int col) {
    if (row >= 0 && col >= 0 && row < occupiedPositions.size() && col < occupiedPositions[0].size())
        occupiedPositions[row][col] = true;
}

//void MazeGenerator::PlaceFood(int count) {
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> disRow(0, maze.size() - 1);
//    std::uniform_int_distribution<> disCol(0, maze[0].size() - 1);
//    std::uniform_int_distribution<> disFloor(0);
//
//    for (int i = 0; i < count; ++i) {
//        int foodRow, foodCol, foodFloor;
//
//        // Find a valid position for food
//        do {
//            foodRow = disRow(gen);
//            foodCol = disCol(gen);
//            foodFloor = disFloor(gen);
//        } while (IsWall(foodRow, foodCol) || IsPositionOccupied(foodRow, foodCol)); // Ensure the position is not a wall or occupied
//
//        // Place the food object
//        Object* food = PlaceModelOnGrid("assets/models/Food/Melon2.ply", foodRow, foodCol, foodFloor, 1.0f * 7.0f, FOOD, true);
//        food->mesh->objectColourRGBA = glm::vec4(1.0f, 0.5f, 0.7f, 1.0f);
//        food->mesh->bOverrideObjectColour = true;
//
//        std::cout << "Pos of food Col " << foodCol << std::endl;
//        std::cout << "Pos of food Row " << foodRow << std::endl;
//
//        // Add the food object to the foods vector
//        if (food != nullptr) {
//            foods.push_back(food);
//            MarkPositionOccupied(foodRow, foodCol); // Mark the position as occupied
//        }
//    }
//}
//
//void MazeGenerator::PlaceWater(int count) {
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> disRow(0, maze.size() - 1);
//    std::uniform_int_distribution<> disCol(0, maze[0].size() - 1);
//    std::uniform_int_distribution<> disFloor(0);
//
//    for (int i = 0; i < count; ++i) {
//        int waterRow, waterCol, waterFloor;
//
//        // Find a valid position for water
//        do {
//            waterRow = disRow(gen);
//            waterCol = disCol(gen);
//            waterCol = disCol(gen);
//            waterFloor = disCol(gen);
//        } while (IsWall(waterRow, waterCol) || IsPositionOccupied(waterRow, waterCol)); // Ensure the position is not a wall or occupied
//
//        // Place the water object
//        Object* water = PlaceModelOnGrid("assets/models/Food/WaterE.ply", waterRow, waterCol, waterFloor, 1.0f * 7.0f, FOOD, true);
//        water->mesh->textures[0] = "Water.bmp";
//        water->mesh->blendRatio[0] = 2;
//        water->mesh->bOverrideObjectColour = false;
//
//        std::cout << "Pos of water Col " << waterCol << std::endl;
//        std::cout << "Pos of water Row " << waterRow << std::endl;
//
//        // Add the water object to the waters vector
//        if (water != nullptr) {
//            waters.push_back(water);
//            MarkPositionOccupied(waterRow, waterCol); // Mark the position as occupied
//        }
//    }
//}
