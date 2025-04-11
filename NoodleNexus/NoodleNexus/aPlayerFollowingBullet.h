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
    float rotationSpeed = 180.0f;  // Degrees per second (max speed)
    float rampUpTime = 2.0f;       // Time (seconds) to reach full rotation speed
    float currentTime = 0.0f;      // Time elapsed since bullet spawn
    float rotationMultiplier = 0.0f; // Starts at 0, ramps up to 1
    float visionRadius = 10.f;
    glm::vec3 currentDirection;    // Normalized movement direction

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
        currentTime = 0.0f; // Reset timer on spawn
        rotationMultiplier = 0.0f; // Start with no rotation
    }

    virtual void Update() override {
        aPlayerBullet::Update(); // Handle base bullet logic and movement
        if (!object || !factory || !projectile || glm::length(projectile->speed) < 0.01f) return;

        // Update rotation multiplier (ramp up over time)
        currentTime += object->scene->deltaTime;
        rotationMultiplier = glm::clamp(currentTime / rampUpTime, 0.0f, 1.0f);

        // Find closest enemy in vision range
        BruteEnemy* closestEnemy = FindClosestEnemy(object->mesh->positionXYZ);
        if (!closestEnemy) return;

        glm::vec3 targetPos = closestEnemy->object->GetWorldPosition() + closestEnemy->colliderCenter;
        glm::vec3 desiredDirection = glm::normalize(targetPos - object->mesh->positionXYZ);

        // Calculate rotation between current and desired direction
        float angle = glm::degrees(glm::angle(currentDirection, desiredDirection));
        float maxAngle = rotationSpeed * rotationMultiplier * object->scene->deltaTime; // Apply multiplier

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
        projectile->UpdateRotation();
    }

    BruteEnemy* FindClosestEnemy(const glm::vec3& position) {
        BruteEnemy* closest = nullptr;
        float closestDistance = visionRadius;

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
};