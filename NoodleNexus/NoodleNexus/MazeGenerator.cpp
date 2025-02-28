#include "MazeGenerator.hpp"
#include "sObject.h"
#include <iostream>
#include <fstream>
#include "PhysicsManager.h"
#include <glm/gtc/matrix_transform.hpp> // For glm::radians
#include <random>


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
            if (c == 'X' || c == '.' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6') {
                row.push_back(c);
            }
        }
        maze.push_back(row);
    }
    file.close();
}

void MazeGenerator::generateMaze() {
    for (size_t row = 0; row < maze.size(); ++row) {
        for (size_t col = 0; col < maze[row].size(); ++col) {
            char cell = maze[row][col];

            if (cell == 'X' || cell == '.' || cell == '3' || cell == '4' || cell == '5' || cell == '6') {
                PlaceModelOnGrid("assets/models/extras/SM_Env_Floor_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, CENTER, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                //PlaceModelOnGrid("assets/models/extras/SM_Env_Ceiling_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, CENTERup, false, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

                // Place surrounding walls
                if (row > 0 && maze[row - 1][col] == 'X') {
                    PlaceModelOnGrid("assets/models/extras/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, DOWN, true, glm::vec4(0.0f, 1.0f, 0.0f, 1.f));
                }
                //if (row < maze.size() - 1 && maze[row + 1][col] == 'X') {
                //    PlaceModelOnGrid("assets/models/extras/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, UP, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                //}
                if (col > 0 && maze[row][col - 1] == 'X') {
                    PlaceModelOnGrid("assets/models/extras/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, LEFT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                }
                //if (col < maze[row].size() - 1 && maze[row][col + 1] == 'X') {
                //    PlaceModelOnGrid("assets/models/extras/SM_Env_Wall_02_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, RIGHT, true, glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
                //}

                // Door placement logic (optional, as in your example)
                if (cell == '3') {
                    Direction doorDirection;
                    if (col > 0 && maze[row][col - 1] == '1') {
                        doorDirection = DOORleft;
                    }
                    else if (col < maze[row].size() - 1 && maze[row][col + 1] == '1') {
                        doorDirection = DOORright;
                    }
                    else if (row > 0 && maze[row - 1][col] == '1') {
                        doorDirection = DOORup;
                    }
                    else if (row < maze.size() - 1 && maze[row + 1][col] == '1') {
                        doorDirection = DOORdown;
                    }
                    else {
                        continue;
                    }
                    PlaceModelOnGrid("assets/models/extras/SM_Env_Door_01_xyz_n_rgba_uv.ply", row, col, 1.0f * 7.0f, doorDirection, true);
                }

                // Place random objects in '1' cells
                if (cell == '4') {
                    PlaceModelOnGrid("", row, col, 1.0f * 7.0f, SMALLobj, true);
                }
                else if (cell == '5') {
                    PlaceModelOnGrid("", row, col, 1.0f * 7.0f, MEDIUMobj, true);
                }
                else if (cell == '6') {
                    PlaceModelOnGrid("", row, col, 1.0f * 7.0f, BIGobj, true);
                }
            }
        }
    }
}



void MazeGenerator::PlaceModelOnGrid(std::string path, int row, int col, float scale, Direction type, bool isVisible, glm::vec4 color) {
    glm::vec3 position(col * scale * 5.0f, 0.0f, row * scale * 5.0f);
    glm::vec3 rotation(0.0f);
    glm::vec3 objectScale(1, 1, 1);


    switch (type) {
    case SMALLobj: {
        std::vector<std::string> smallObjectPaths = {
            "assets/models/extras/SM_Prop_Plant_01_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_Bed_02_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_Chair_01_xyz_n_rgba_uv.ply"
        };
        path = smallObjectPaths[rand() % smallObjectPaths.size()];
        scale *= 1.f;  // Smaller scale for small objects
        color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        break;
    }
    case MEDIUMobj: {
        std::vector<std::string> mediumObjectPaths = {
            "assets/models/extras/SM_Prop_ControlDesk_01_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_Console_05_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_StepLadder_01_xyz_n_rgba_uv.ply"
        };
        path = 1.f;  // Medium scale for medium objects
        color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
        break;
    }
    case BIGobj: {
        std::vector<std::string> bigObjectPaths = {
            "assets/models/extras/SM_Prop_3DPrinter_01_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_Treadmill_01_xyz_n_rgba_uv.ply",
            "assets/models/extras/SM_Prop_Stairs_01_xyz_n_rgba_uv.ply"
        };
        path = bigObjectPaths[rand() % bigObjectPaths.size()];
        scale *= 1.2f;  // Larger scale for big objects
        color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        break;
    }
    case CENTER:
        position.z -= 2.5f * scale;
        position.x += 2.5f * scale;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        break;
    case CENTERup:
        position.y += scale * 5.0f;
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        break;
    case RIGHT:
        position.x += scale * 5.0f / 2.0f;
        rotation.y = -90.0f;
        color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        break;
    case LEFT:
        position.x -= scale * 5.0f / 2.0f;
        rotation.y = -270.0f;
        color = glm::vec4(0.3f, 0.6f, 0.3f, 1.0f);
        break;
    case UP:
        position.z += scale * 5.0f / 2.0f;
        rotation.y = 180.0f;
        break;
    case DOWN:
        position.z -= scale * 5.0f / 2.0f;
        break;
    case DOORleft:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        position.x -= scale * 0.5f;
        rotation.y = 0.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORright:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        position.x -= scale * -2.5f;
        rotation.y = 180.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORup:
        position.z += scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        rotation.y = 90.0f;
        scale = 7.0f * 1.6f;
        break;
    case DOORdown:
        position.z -= scale * 5.0f / 2.0f;
        position.y -= scale * 1.5f;
        rotation.y = -90.0f;
        scale = 7.0f * 1.6f;
        break;
    default:
        std::cerr << "Unknown ObjectType." << std::endl;
        return;
    }

    // Generate the object with the applied scale to actually affect the mesh size
    Object* obj = scene->GenerateMeshObjectsFromObject(
        path, position, scale * objectScale.x, rotation,false, glm::vec4(1.0, 0.2, 0.2, 1.0f), true, scene->sceneObjects);


    if (obj == nullptr) {
        std::cerr << "Failed to create object for type: " << type << std::endl;
        return;
    }

    obj->isTemporary = true;
    obj->mesh->drawBothFaces = true;
    obj->mesh->textures[0] = "rock.bmp";
    obj->mesh->blendRatio[0] = 1;
    obj->mesh->textureFillType[0] = 1;
    // Set visibility for invisible walls
    if (!isVisible) {
        obj->mesh->bIsVisible = false;
    }


    // Add to physics manager with the applied scale
    scene->physicsManager->AddTriangleMesh(path, position, rotation, scale);
}

