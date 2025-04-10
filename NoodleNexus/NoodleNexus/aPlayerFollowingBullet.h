#pragma once
#include "aBullet.h"
#include "LabAttackFactory.h"
#include "BruteEnemy.h"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

class aPlayerFollowingBullet : public aPlayerBullet {
private:
    float visionRange = 100.0f;    // Detection range for enemies
    float rotationSpeed = 180.0f;  // Degrees per second
    glm::vec3 currentDirection;   // Normalized movement direction


public:

    LabAttackFactory* factory = nullptr;
    aProjectileMovement* projectile = nullptr;  // Reference to movement component

    virtual void Start() override {
        aPlayerBullet::factory = factory;
        aPlayerBullet::projectile = projectile;
        aPlayerBullet::Start();
        if (projectile) {
            currentDirection = glm::normalize(projectile->speed);
        }
    }

    virtual void Update() override {
        aPlayerBullet::Update(); // Handle base bullet logic and movement
      
        if (!factory || !projectile || glm::length(projectile->speed) < 0.01f) return;

        // Find closest enemy in vision range
        BruteEnemy* closestEnemy = nullptr;
        float closestDistance = FLT_MAX;
        glm::vec3 bulletPos = object->mesh->positionXYZ;

        for (BruteEnemy* enemy : factory->m_creepPool) {
            if (!enemy->object->isActive) continue;

            glm::vec3 enemyPos = enemy->object->GetWorldPosition() + enemy->colliderCenter;
            float distance = glm::distance(bulletPos, enemyPos);

            if (distance < visionRange && distance < closestDistance) {
                closestDistance = distance;
                closestEnemy = enemy;
            }
        }

        if (closestEnemy) {
            glm::vec3 targetPos = closestEnemy->object->GetWorldPosition() + closestEnemy->colliderCenter;
            glm::vec3 desiredDirection = glm::normalize(targetPos - bulletPos);

            // Calculate rotation between current and desired direction
            float angle = glm::degrees(glm::angle(currentDirection, desiredDirection));
            float maxAngle = rotationSpeed * object->scene->deltaTime;

            if (angle > maxAngle) {
                // Rotate towards target by maximum allowed angle
                glm::vec3 rotationAxis = glm::normalize(glm::cross(currentDirection, desiredDirection));
                glm::quat rot = glm::angleAxis(glm::radians(maxAngle), rotationAxis);
                currentDirection = rot * currentDirection;
            }
            else {
                currentDirection = desiredDirection;
            }

            // Update projectile speed while maintaining velocity magnitude
            float originalSpeed = glm::length(projectile->speed);
            projectile->speed = currentDirection * originalSpeed;

            // Update projectile rotation
            projectile->UpdateRotation();
        }

        // Optional: Add visual rotation to projectile model
     //   object->mesh->rotationEulerXYZ += glm::vec3(0.0f, 5.0f, 0.0f) * rotationSpeed * object->scene->deltaTime;
    }

    BruteEnemy* FindClosestEnemy(const glm::vec3& position) {
        BruteEnemy* closest = nullptr;
        float closestDistance = FLT_MAX;

        for (BruteEnemy* enemy : factory->m_creepPool) {
            if (!enemy->object->isActive) continue;

            float distance = glm::distance(position,
                enemy->object->GetWorldPosition() + enemy->colliderCenter);

            if (distance < visionRange && distance < closestDistance) {
                closestDistance = distance;
                closest = enemy;
            }
        }
        return closest;
    }

    void UpdateDirection(const glm::vec3& newDirection) {
        currentDirection = glm::normalize(newDirection);
        if (projectile) {
            float originalSpeed = glm::length(projectile->speed);
            projectile->speed = currentDirection * originalSpeed;
            projectile->UpdateRotation();
        }
    }
};