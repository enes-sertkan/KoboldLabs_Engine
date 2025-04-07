#pragma once
#include "sObject.h"
#include "Action.h"
#include "LabAttackFactory.h"
#include "aProjectileMovement.hpp"

class aBullet : public Action {
private:
    aProjectileMovement* projectile = new aProjectileMovement();

    float lifetime = 0.0f;
    const float maxLifetime = 3.0f; // Self-destruct after 3 seconds

public:
    LabAttackFactory* factory = nullptr;

    virtual void Start() {
        lifetime = 0.0f; // Reset timer on spawn
    }

    virtual void Update() {
       
        // 2. Lifetime tracking
        lifetime += object->scene->deltaTime;
        if (lifetime >= maxLifetime) {
            object->Destroy(); // Self-destruct after timeout
        }

        if (!projectile || !factory) return;

        std::vector<sCollision_RayTriangleInMesh> collisions;
        glm::vec3 pos = object->GetWorldPosition();

      
        glm::vec3 sadFix = pos;
        sadFix.x += factory->maze->TILE_SIZE * 0.45;
        sadFix.z += factory->maze->TILE_SIZE * 0.6;

        glm::vec2 mazePos = factory->maze->WorldToGrid(sadFix);


        if (factory->maze->IsWall(mazePos.y, mazePos.x))
        {
            object->Destroy(); // Self-destruct after timeout
        }
    }
};