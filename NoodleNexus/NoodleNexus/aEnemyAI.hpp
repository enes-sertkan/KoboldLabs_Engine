#pragma once
#include "Action.h"
#include "Scene.hpp"
#include "aPlayer.hpp" // Include the aPlayer class

class aEnemyAI : public Action {
protected:
    Object* player;
    int health = 3;

    // Helper function to find the player object
    Object* FindPlayer() {
        for (auto obj : object->scene->sceneObjects) {
            if (obj->GetActionOfType<aPlayer>() != nullptr) {
                return obj; // Found the player
            }
        }
        return nullptr; // Player not found
    }

    glm::vec3 GetPlayerPosition() {
        return player ? player->mesh->positionXYZ : glm::vec3(0);
    }

public:
    void Start() override {
        player = FindPlayer(); // Dynamically find the player object
    }

    void TakeDamage(int damage) {
        health -= damage;
        if (health <= 0) {
            object->Destroy(); // Destroy the enemy if health reaches 0
        }
    }
};