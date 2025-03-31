#pragma once
#include "aAgent.h"
#include "MoveToPlayer.h"
#include "ShootAtPlayerAction.h"
#include "MoveToControlPointAction.h"


class BruteEnemy : public Agent {
public:
    BruteEnemy() {
        speed = 4.f;
       // attackDamage = 30.0f;
        attackRange = 3.f;
        health = 200;

        // Available actions
        availableActions.push_back(new MoveToPlayerAction());
        availableActions.push_back(new ShootAtPlayerAction());
        availableActions.push_back(new MoveToControlPointAction());
  

        // Default goal: Get in range and attack
        goal = { {"hasReachedControlPoint", true} };
    }

    void updateWorldState() override {
        // Global knowledge + local tweaks
        worldState["playerVisible"] = (true);
        worldState["playerInRange"] = (glm::distance(object->mesh->positionXYZ, maze->player->mesh->positionXYZ) <= attackRange);
    }
};