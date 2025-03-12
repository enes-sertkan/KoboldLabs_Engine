#pragma once
#include "aEnemyAI.hpp"
#include <cmath> // For atan2 and M_PI

class Enemy2AI : public aEnemyAI {
private:
    float pursueSpeed = 4.0f;
    float evadeSpeed = 6.0f;

public:
    void Update() override {
        if (!player) return;

        PursuePlayer();
        EvadeBullets();
    }

    void PursuePlayer() {
        glm::vec3 toPlayer = GetPlayerPosition() - object->mesh->positionXYZ;
        object->mesh->positionXYZ += glm::normalize(toPlayer) * pursueSpeed * object->scene->deltaTime;

        // Rotate the enemy towards the player
        RotateTowards(toPlayer);
    }

    void EvadeBullets() {
        // Find the closest bullet to the enemy
        Object* closestBullet = FindClosestBullet();
        if (closestBullet) {
            // Calculate the direction to move away from the bullet
            glm::vec3 evadeDir = object->mesh->positionXYZ - closestBullet->mesh->positionXYZ;

            // Move the enemy in the opposite direction of the closest bullet
            object->mesh->positionXYZ += glm::normalize(evadeDir) * evadeSpeed * object->scene->deltaTime;

            // Rotate the enemy to face the opposite direction of the bullet
            RotateTowards(evadeDir);
        }
    }

    Object* FindClosestBullet() {
        Object* closestBullet = nullptr;
        float closestDistance = FLT_MAX;

        // Get the player's items controller
        aPlayerItemsController* playerItems = player->GetActionOfType<aPlayerItemsController>();
        if (!playerItems) return nullptr; // Player items controller not found

        // Loop through all bullets and find the closest one
        for (Object* bullet : playerItems->playerBullets) {
            if (bullet) {
                float distance = glm::length(bullet->mesh->positionXYZ - object->mesh->positionXYZ);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestBullet = bullet;
                }
            }
        }

        return closestBullet;
    }

private:
    void RotateTowards(const glm::vec3& direction) {
        // Calculate the angle in radians based on the movement direction
        float angle = atan2(direction.x, direction.z);

        // Convert radians to degrees (if your rotation system uses degrees)
        float angleDegrees = glm::degrees(angle);

        // Update the rotation Euler angles (yaw is around the Y-axis)
        object->mesh->rotationEulerXYZ.y = angleDegrees;
    }
};