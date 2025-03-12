#pragma once
#include "aEnemyAI.hpp"
#include "Gun.hpp"  // Include the Gun class for shooting
#include <cmath>    // For atan2 and M_PI

class Enemy3AI : public aEnemyAI {
private:
    float approachSpeed = 3.0f;
    float shootInterval = 2.0f;
    float shootTimer = 0.0f;
    float combatRadius = 5.0f;
    glm::vec3 bulletDirection;

public:
    void Update() override {
        if (!player) return;

        MaintainDistance();
        ShootAtPlayer();
    }

    void MaintainDistance() {
        glm::vec3 toPlayer = GetPlayerPosition() - object->mesh->positionXYZ;
        float distance = glm::length(toPlayer);

        if (distance > combatRadius) {
            // Move towards the player and rotate to face the player
            object->mesh->positionXYZ += glm::normalize(toPlayer) * approachSpeed * object->scene->deltaTime;
            RotateTowards(toPlayer);
        }
        else if (distance < combatRadius - 1.0f) {
            // Move away from the player and rotate to face the player
            object->mesh->positionXYZ -= glm::normalize(toPlayer) * approachSpeed * object->scene->deltaTime;
            RotateTowards(toPlayer);
        }
    }

    void ShootAtPlayer() {
        shootTimer -= object->scene->deltaTime;
        if (shootTimer <= 0) {
            // Calculate direction to the player
            glm::vec3 toPlayer = GetPlayerPosition() - object->mesh->positionXYZ;
            bulletDirection = glm::normalize(toPlayer);  // Direction of the bullet towards the player

            // Rotate to face the player before shooting
            RotateTowards(toPlayer);

            // Create a projectile (similar to the Gun::Use() logic)
            std::cout << "Enemy shooting projectile!" << std::endl;

            Object* projectileAI = object->scene->GenerateMeshObjectsFromObject(
                "assets/models/Cube_xyz_n_uv.ply",
                object->mesh->positionXYZ,
                1.0f,
                glm::vec3(0.0f, 0.0f, 0.0f),
                false,
                glm::vec4(0.1, 0.1, 0.5, 1),
                false,
                object->scene->sceneObjects
            );
            projectileAI->isTemporary = true;

            // Attach actions to the projectile
            aExplodeOnRayCollision* explodeOnRayCollisionAI = new aExplodeOnRayCollision();
            explodeOnRayCollisionAI->rayDirection = bulletDirection;
            explodeOnRayCollisionAI->rayLenght = 1.9f;
            object->scene->AddActionToObj(explodeOnRayCollisionAI, projectileAI);

            // Second explosion check (can be used for alternate direction or functionality)
            aExplodeOnRayCollision* explodeOnRayCollision2AI = new aExplodeOnRayCollision();
            explodeOnRayCollision2AI->rayDirection = glm::vec3(0, -1, 0);
            explodeOnRayCollision2AI->rayLenght = 1.9f;
            object->scene->AddActionToObj(explodeOnRayCollision2AI, projectileAI);

            // Set projectile physics and motion
            RayCastPhysics* physAI = new RayCastPhysics();
            physAI->gravityAcceleration.y = -1;
            physAI->baseRayCastLength = 5.0;
            physAI->speed = bulletDirection * 6.0f;  // Bullet moves in the direction of the player
            physAI->bounciness = 1.5f;
            object->scene->AddActionToObj(physAI, projectileAI);

            // Reset the shooting timer
            shootTimer = shootInterval;
        }
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
