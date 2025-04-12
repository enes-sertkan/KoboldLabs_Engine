#pragma once
#include "Action.h"
#include "cTurret.h"
#include "LabAttackFactory.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/intersect.hpp>

class aTurretPlacer : public Action {
public:
    LabAttackFactory* factory = nullptr;
    Turret* ghostTurret = nullptr;
    bool isActive = true;
    bool isValidPos = false;

    enum eSelectedPart { HEAD, NECK, BODY } currentSelection = BODY;
    float keyCooldown = 0.2f;
    float currentCooldown = 0.0f;


    void Start() override {
        // Create ghost turret instance at hidden position
        ghostTurret = factory->SpawnTurretGhost(glm::vec3(-100), STANDARTBODY, STANDARTNECK, STANDARTHEAD);
        UpdateWireframe();
    }

    void SetActive(bool active) {
        isActive = active;
        // Move to valid position when activating
        if (active) UpdateGhostPosition();
    }

    void OnDestroy() override {
        if (ghostTurret) {
         //   ghostTurret->RebuildTurretGhost(nullptr);
            delete ghostTurret;
        }
    }

    void Update() override {
        if (!ghostTurret || !factory) return;

        currentCooldown -= object->scene->deltaTime;

        UpdateGhostPosition();
        HandleInput();
        UpdateWireframe();
    }

private:
    void HandleInput() {
        if (currentCooldown > 0) return;


        //if (glfwGetKey(object->scene->window, GLFW_KEY_Q) == GLFW_PRESS) {
        //    SetActive(!isActive);
        //    currentCooldown = keyCooldown;
        //}

        if (!isActive) return;

        // Selection cycling
        if (glfwGetKey(object->scene->window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            currentSelection = static_cast<eSelectedPart>((currentSelection + 1) % 3);
            currentCooldown = keyCooldown;
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_UP) == GLFW_PRESS) {
            currentSelection = static_cast<eSelectedPart>((currentSelection - 1 + 3) % 3);
            currentCooldown = keyCooldown;
        }

        // Part modification
        if (glfwGetKey(object->scene->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            ChangePart(-1);
            currentCooldown = keyCooldown;
        }
        if (glfwGetKey(object->scene->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            ChangePart(1);
            currentCooldown = keyCooldown;
        }

        // Placement
        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !object->scene->isFlyCamera) {
            PlaceTurret();
            currentCooldown = keyCooldown;
        }

        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !object->scene->isFlyCamera) {
            PlaceTurret();
            currentCooldown = keyCooldown;
        }
    }

    void UpdateWireframe() {
        if (!ghostTurret) return;

        // Reset all wireframes
        if (ghostTurret->body && ghostTurret->body->object)
            ghostTurret->body->object->mesh->bIsWireframe = false;
        if (ghostTurret->neck && ghostTurret->neck->object)
            ghostTurret->neck->object->mesh->bIsWireframe = false;
        if (ghostTurret->head && ghostTurret->head->object)
            ghostTurret->head->object->mesh->bIsWireframe = false;

        // Highlight selected part
        switch (currentSelection) {
        case BODY:
            if (ghostTurret->body && ghostTurret->body->object)
                ghostTurret->body->object->mesh->bIsWireframe = true;
            break;
        case NECK:
            if (ghostTurret->neck && ghostTurret->neck->object)
                ghostTurret->neck->object->mesh->bIsWireframe = true;
            break;
        case HEAD:
            if (ghostTurret->head && ghostTurret->head->object)
                ghostTurret->head->object->mesh->bIsWireframe = true;
            break;
        }
    }

    void ChangePart(int direction) {
        sTurretCofig* config = ghostTurret->GetConfig();

        switch (currentSelection) {
        case HEAD: config->headID = CycleEnum(config->headID, direction); 
            std::cout<<"Head changed to " << config->headID << std::endl; break;
        case NECK: config->neckID = CycleEnum(config->neckID, direction); 
            std::cout << "Neck changed to " << config->neckID << std::endl; break; 
        case BODY: config->bodyID = CycleEnum(config->bodyID, direction); 
            std::cout << "Body changed to " << config->bodyID << std::endl; break;
        }

        ghostTurret->RebuildTurretGhost(config);
        UpdateWireframe();
    }

    template<typename T>
    T CycleEnum(T current, int direction) {
        const int enumCount = EnumInfo<T>::count();
        int newValue = (static_cast<int>(current) + direction + enumCount) % enumCount;
        return static_cast<T>(newValue);
    }


    void UpdateGhostPosition() {
        if (!isActive) {
            // Keep ghost hidden when inactive
            ghostTurret->body->object->mesh->positionXYZ = glm::vec3(-100);
            return;
        }

        glm::vec3 planeNormal(0, 1, 0);
        Camera* camera = object->scene->fCamera->getCameraData();
        glm::vec3 rayStart = camera->position;

        // Calculate camera forward
        glm::vec3 cameraRotation = camera->rotation;
        float pitch = glm::radians(cameraRotation.x);
        float yaw = glm::radians(cameraRotation.y);

        glm::vec3 rayDir;
        rayDir.x = cos(pitch) * cos(yaw);
        rayDir.y = sin(pitch);
        rayDir.z = cos(pitch) * sin(yaw);
        rayDir = glm::normalize(rayDir);

        isValidPos = true;
        float distance;
        if (glm::intersectRayPlane(rayStart, rayDir, glm::vec3(0, 3.1, 0), planeNormal, distance) && distance > 0) {
            glm::vec3 newPos = rayStart + rayDir * distance;
            glm::vec3 sadFix = newPos;
            sadFix.x += factory->maze->TILE_SIZE*0.45;
            sadFix.z += factory->maze->TILE_SIZE*0.6;

            glm::vec2 mazePos = factory->maze->WorldToGrid(sadFix);

            isValidPos = !factory->maze->IsWall(mazePos.y, mazePos.x);


            for (Turret* tur : factory->turrets)
            {
                float dis = glm::distance(tur->body->object->GetWorldPosition(), newPos);

                    if (dis < 1.4)
                    {
                        isValidPos = false;
                        break;
                    }

            }

            if (isValidPos)
            ghostTurret->body->object->mesh->positionXYZ = newPos;
            else
            ghostTurret->body->object->mesh->positionXYZ = glm::vec3(-100);
  
        }
    }


    void PlaceTurret() {
        if (!isActive || !factory || !ghostTurret || !isValidPos) return;

        sTurretCofig* config = ghostTurret->GetConfig();
        factory->SpawnTurret(
            ghostTurret->body->object->mesh->positionXYZ,
            config->bodyID,
            config->neckID,
            config->headID
        );
    }
};