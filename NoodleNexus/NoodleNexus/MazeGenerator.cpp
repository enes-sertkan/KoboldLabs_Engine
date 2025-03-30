#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians
#include <random>
#include "aMinotaur.h"
#include "Theseus.h"
#include "aSoftBodyAction.hpp"
#include "BruteEnemy.h"
#include "aEnemySpawner.h"
#include "LabAttackFactory.h"
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
         //   if (c == 'X' || c == '.' || c == 'T' || c == 'M' || c == 'B' || c == 'R' || c == 'V') { //WHYYYYY?
                row.push_back(c);
         //   }
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

            if (cell == '.' || cell == 'M' || cell == 'T' || cell == 'R' ||  cell == 'S' || cell == 'B') {

                if (cell != 'S')
                {
                    PlaceModelOnGrid("assets/models/objects/floor.ply", row, col, floor, 1.0f, CENTER, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                else
                {
                        PlaceModelOnGrid("assets/models/objects/floor_jumpy.ply", row, col, floor, 1.0f, SOFTCENTER, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
      


               // PlaceModelOnGrid("assets/models/objects/floor.ply", row, col, floor, 1.0f, CENTERup, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

                // Place surrounding walls
                if (row > 0 && maze[row - 1][col] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor, 1.0f, DOWN, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f));
                  //  PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor + 1.0f, 1.0f, DOWN, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f));
                }
                if (row < maze.size() - 1 && maze[row + 1][col] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor, 1.0f, UP, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                   // PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor + 1.0f, 1.0f, UP, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                if (col > 0 && maze[row][col - 1] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor, 1.0f, LEFT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                 //   PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor + 1.0f, 1.0f, LEFT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                if (col < maze[row].size() - 1 && maze[row][col + 1] == 'X') {
                    PlaceModelOnGrid("assets/models/objects/castleWall1.ply", row, col, floor, 1.0f, RIGHT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                 //   PlaceModelOnGrid("assets/models/objects/wall01side.ply", row, col, floor + 1.0f, 1.0f, RIGHT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }




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
                    PlaceModelOnGrid("", row, col, floor, 1.0f, BRUTEENEM, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                else if (cell == 'R') {
                    PlaceModelOnGrid("", row, col, floor, 1.0f * 7.0f, REACTORS, true);
                }

                
            }

            if (cell >= '1' && cell <= '9') {
                controlPoints.push_back(glm::vec2(col, row));
            }
        }
    }
    //PlaceFood(50);
    //PlaceWater(100);
}
bool aobool = false;
Object* MazeGenerator::PlaceModelOnGrid(std::string path, int row, int col, int floor, float scale, Direction type, bool isVisible, glm::vec4 color) {
    glm::vec3 position(col * scale * 4.0f, floor * scale * 3.0f, row * scale * 4.0f);
    glm::vec3 rotation(0.0f);
    glm::vec3 objectScale(1, 1, 1);
    std::string texture;
    bool isOverrideColor = false;
    bool useAO = false;
    std::string textureAO="";
    std::string textureST="";
    std::string textureNM="";
    float metal = 0;
    float smothness = 0.2;
    switch (type) {
    case SOFTCENTER:
        position.z -= 2.5f * scale;
        position.x += 2.5f * scale;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        texture = "Floor_Albedo.bmp";
        useAO = true;
        //   aobool = !aobool;
        textureAO = "WallAO.bmp";
        //textureNM = "Floor_Normal.bmp";
        //  textureST = "Operating_Table_MetallicSmoothness.bmp";
        metal = 1.f;
        break;
    case CENTER:
        position.z -= 2.5f * scale;
        position.x += 2.5f * scale;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        texture = "Floor_Albedo.bmp";
        useAO = true;
     //   aobool = !aobool;
        textureAO = "WallAO.bmp";
        textureNM = "Floor_Normal.bmp";
        metal = 0.1;
        smothness = 0.2;
      //  textureST = "Operating_Table_MetallicSmoothness.bmp";
        break;
    case CENTERup:
        position.z += 1.5f * scale;
        position.x += 2.5f * scale;
        position.y += 6.0f * scale;
       // rotation.x = 180;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        textureAO = "WallAO.bmp";
        texture = "Floor_Albedo.bmp";
        textureNM = "Floor_Normal.bmp";
        metal = 0.1;
        smothness = 0.2;
        break;
    case RIGHT:
        position.x += scale * 5.f/ 2.0f;
        position.z -= scale * 5.f / 2.0f;
        rotation.y = 90.f;
        color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        texture = "castle_element_05_BaseColour.bmp";
        textureST = "castle_element_05_MetalSmoothness.bmp";
       textureNM = "castle_element_05_NormalGL.bmp";
        break;
    case LEFT:
        position.x -= scale * 3.9f / 2.0f;
        position.z -= scale * 5.0f / 2.0f;
        color = glm::vec4(0.3f, 0.6f, 0.3f, 1.0f);
        rotation.y = 90.f;
        texture = "castle_element_05_BaseColour.bmp";
        textureST = "castle_element_05_MetalSmoothness.bmp";
        textureNM = "castle_element_05_NormalGL.bmp";
        break;
    case UP:
        position.z += scale * 3.9f / 2.0f;
        position.x += scale * 5.0f / 2.0f;
       // rotation.y = 180.f;
       texture = "castle_element_05_BaseColour.bmp";
        textureST = "castle_element_05_MetalSmoothness.bmp";
       textureNM = "castle_element_05_NormalGL.bmp";
       metal = 0.f;
        break;
    case DOWN:
        position.z -= scale * 5.f / 2.0f;
        position.x += scale * 5.f / 2.0f;
        texture = "castle_element_05_BaseColour.bmp";
       textureST = "castle_element_05_MetalSmoothness.bmp";
      textureNM = "castle_element_05_NormalGL.bmp";
        break;
    case BRUTEENEM:
        position.y += 3.f;
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

    if (type == BRUTEENEM)
    {
    
        obj->name = "ENEMY SPAWNER";
        aEnemySpawner* spawner = new aEnemySpawner();
   
        spawner->factory = factory;
        isOverrideColor = true;
        scene->AddActionToObj(spawner, obj);

    }

    if (type == SOFTCENTER)
    {
        obj->mesh->uniformScale = 2.f;
        obj->mesh->metal = 0.7;
        obj->mesh->smoothness = 0.7;
        SoftBody* softBody = new SoftBody();
        softBody->isLockOutsideRadius = true;
        softBody->lockRadius = 1.8f;
        softBody->checkGreaterZLock = true;
        scene->AddActionToObj(softBody, obj);
        softBody->acceleration.y = -5.f;
        softBody->constIterations = 10;
        softBody->tighness = 1.5;
        softBody->yToJump = -8.5;
        //softBody->restLengthMultiplier = 2.f;
        softBody->sbCollision->collisionMult = 0.4f;
        if (mainSlime != nullptr)
        {
           softBody->AddSoftBodyToCollisions(mainSlime);
            mainSlime->AddSoftBodyToCollisions(softBody);
            
        }

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
    obj->mesh->metal = metal;
    obj->mesh->smoothness = smothness;
    obj->mesh->AOtexture = textureAO;
    obj->mesh->STTexture = textureST;
    obj->mesh->NMTexture = textureNM;
    obj->mesh->metal=1.f;
   // obj->mesh->useAO = useAO;
   // Set visibility for invisible walls
    if (!isVisible) {
        obj->mesh->bIsVisible = false;
    }


    // Add to physics manager with the applied scale
    scene->physicsManager->AddTriangleMesh(path, position, rotation, scale);

    return obj;
}

bool MazeGenerator::IsWall(int x, int y)  {
    if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size())
        return true;
    return maze[x][y] == 'X';
}

bool MazeGenerator::IsFloor(int x, int y)
{
    if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size())
        return true;
    return maze[x][y] != 'S' && maze[x][y] != ',';
}

glm::vec3 MazeGenerator::GridToWorld(int x, int y)  {
    const float TILE_SIZE = 1.0f*4.f; // Match your scaling factor
    return glm::vec3(x * TILE_SIZE, 0, y * TILE_SIZE);
}

char MazeGenerator::GetMazePoint(int x, int y) {
    return maze[x][y];
}

bool MazeGenerator::IsPositionOccupied(int row, int col)  {
    if (row < 0 || col < 0 || row >= occupiedPositions.size() || col >= occupiedPositions[0].size())
        return true; // Out of bounds is considered occupied
    return occupiedPositions[row][col];
}

// Mark a position as occupied
void MazeGenerator::MarkPositionOccupied(int row, int col) {
    if (row >= 0 && col >= 0 && row < occupiedPositions.size() && col < occupiedPositions[0].size())
        occupiedPositions[row][col] = true;
}

