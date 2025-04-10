#pragma once
#include "aAgent.h"
#include "MoveToPlayer.h"
#include "ShootAtPlayerAction.h"
#include "MoveToControlPointAction.h"
#include "aLaunchAction.h" 

class BruteEnemy : public Agent {
public:
    MoveToControlPointAction* mazeMovection = nullptr;
    Object* partToLaunch = nullptr;
    BruteEnemy() {
        speed = 4.f;
       // attackDamage = 30.0f;
        attackRange = 3.f+static_cast<float>(rand()) /
            (static_cast<float>(RAND_MAX / (5.f)));;
        maxHealth = 40;
        health = 40;

        // Available actions
        availableActions.push_back(new MoveToPlayerAction());
        availableActions.push_back(new ShootAtPlayerAction());
        mazeMovection = new MoveToControlPointAction();
        availableActions.push_back(mazeMovection);
        playerDetectionRange = 5.0f + static_cast<float>(rand()) /
            (static_cast<float>(RAND_MAX / (10.0f)));

        // Default goal: Get in range and attack
        goal = { {"hasReachedControlPoint", true} };
    }

    void ResetPath() override
    {
        mazeMovection->reset();
    }

    void updateWorldState() override {
        // Global knowledge + local tweaks
        worldState["playerVisible"] = (true);
        worldState["playerInRange"] = (glm::distance(object->mesh->positionXYZ, maze->player->mesh->positionXYZ) <= attackRange);
    }

    virtual void Death() override {
        // If there is a part to launch, detach it and add a LaunchAction.
        if (partToLaunch) {
            // Unparent the part.
            partToLaunch->RemoveParent();
            // Add the launch action, setting the floor height (for example, y = 0.0f).
            float floorHeight = 0.0f;
            object->scene->AddActionToObj(new aLaunchAction(partToLaunch, floorHeight), partToLaunch);
        }

        // Remove this enemy from the factory's pool.
        std::vector<BruteEnemy*>::iterator it = std::find(factory->m_creepPool.begin(), factory->m_creepPool.end(), this);
        if (it != factory->m_creepPool.end()) {
            factory->m_creepPool.erase(it);
        }

        // Destroy the enemy's object.
        object->Destroy();
    }
};