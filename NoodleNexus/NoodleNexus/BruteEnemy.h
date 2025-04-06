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
        playerDetectionRange = 5.0f + static_cast<float>(rand()) /
            (static_cast<float>(RAND_MAX / (10.0f)));

        // Default goal: Get in range and attack
        goal = { {"hasReachedControlPoint", true} };
    }

    void updateWorldState() override {
        // Global knowledge + local tweaks
        worldState["playerVisible"] = (true);
        worldState["playerInRange"] = (glm::distance(object->mesh->positionXYZ, maze->player->mesh->positionXYZ) <= attackRange);
    }

    void OnDamage(int damage) override
    {
        object->mesh->objectColourRGBA = glm::vec4((maxHealth - health) / maxHealth,0.2,  health / maxHealth, 1.f);
    }

    virtual void Death() override
    {
        std::vector<BruteEnemy*>::iterator it = std::find(factory->m_creepPool.begin(), factory->m_creepPool.end(), this);
        factory->m_creepPool.erase(it);
        object->Destroy();
    }
};