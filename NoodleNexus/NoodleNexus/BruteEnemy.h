#pragma once
#include "aAgent.h"
#include "MoveToPlayer.h"
class BruteEnemy : public Agent {
public:
    BruteEnemy() {
        speed = 1.f;
       // attackDamage = 30.0f;
        attackRange = 1.5f;
        health = 200;

        // Available actions
        availableActions.push_back(new MoveToPlayerAction());
  

        // Default goal: Get in range and attack
        goal = { {"playerInAttackRange", true} };
    }

    void updateWorldState() override {
        // Global knowledge + local tweaks
        worldState["playerVisible"] = (glm::distance(object->mesh->positionXYZ, maze->player->mesh->positionXYZ) < 20.0f);
        worldState["playerInAttackRange"] = (glm::distance(object->mesh->positionXYZ, maze->player->mesh->positionXYZ) <= attackRange);
    }
};