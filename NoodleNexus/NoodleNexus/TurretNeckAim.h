#pragma once
// TurretNeckAim.h
#include "aTurretNeck.h"
#include "LabAttackFactory.h"
#include <glm/gtx/vector_angle.hpp>
#include "sObject.h"
#include "aAgent.h"
#include "BruteEnemy.h"

class TurretNeckAim : public aTurretNeck {
public:
    float rotationSpeed = 90.0f; // Degrees per second
    float maxPitch = 180.0f;      // Maximum vertical aiming angle
    float maxYaw = 180.0f;        // Maximum horizontal aiming angle

    void Update() override {
        if (!object || !factory) return;

        // First do base neck rotation
        aTurretNeck::Update();

        // Then handle aiming
        if (headConnection && headConnection->mesh) {
            Object* target = FindClosestEnemy();
            if (target) {
                AimAtTarget(target);
            }
        }
    }

    Object* FindClosestEnemy() {
        glm::vec3 ourPos = headConnection->GetWorldPosition();
        float closestDist = FLT_MAX;
        Object* closestEnemy = nullptr;

        // Manually check each enemy pool
        CheckEnemyPool(factory->m_creepPool, ourPos, closestDist, closestEnemy);


        return closestEnemy;
    }

    void AimAtTarget(Object* target) {
        if (!target || !headConnection || !object) return;

        glm::vec3 targetPos = target->GetWorldPosition();
        glm::vec3 ourPos = headConnection->GetWorldPosition();

        // Calculate direction to target in world space
        glm::vec3 direction = glm::normalize(targetPos - ourPos);

        // 1. Handle horizontal rotation (neck Y axis)
        glm::vec3 horizontalDir = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

        // Calculate target yaw angle (degrees)
        float targetYaw = atan2(horizontalDir.x, horizontalDir.z) * glm::degrees(1.0f);

        // Get current neck rotation (degrees)
        float currentYaw = object->mesh->rotationEulerXYZ.y;

        // Calculate rotation difference (degrees)
        float yawDifference = targetYaw - currentYaw;

        // Normalize angle to shortest path
        if (yawDifference > 180.0f) yawDifference -= 360.0f;
        if (yawDifference < -180.0f) yawDifference += 360.0f;

        // Apply rotation with speed limit (degrees)
        float yawDelta = glm::clamp(yawDifference, -rotationSpeed * object->scene->deltaTime, rotationSpeed * object->scene->deltaTime);
        object->mesh->rotationEulerXYZ.y += yawDelta;

        // 2. Handle vertical aiming (head connection X/Z rotations)
        // Convert current neck rotation to quaternion (convert degrees to radians for glm)
        glm::quat neckRot = glm::quat(glm::radians(glm::vec3(0, object->mesh->rotationEulerXYZ.y, 0)));

        // Convert direction to neck's local space
        glm::vec3 localDir = glm::inverse(neckRot) * direction;

        // Calculate pitch (vertical angle in degrees)
        float pitch = asin(-localDir.y) * glm::degrees(1.0f);
        pitch = glm::clamp(pitch, -maxPitch, maxPitch);

        // Calculate roll compensation if needed (degrees)
        float roll = atan2(localDir.x, localDir.z) * glm::degrees(1.0f);
        roll = glm::clamp(roll, -maxYaw, maxYaw);

        // Apply vertical aiming to head connection (degrees)
        headConnection->mesh->rotationEulerXYZ = glm::vec3(
            pitch,  // X rotation (pitch up/down)
            0.0f,   // Y rotation 
            roll    // Z rotation (fine horizontal adjustment)
        );
    }

    aTurretNeck* Clone() const override {
        TurretNeckAim* clone = new TurretNeckAim(*this);
        clone->object = nullptr;
        clone->headConnection = nullptr;
        clone->factory = factory;
        return clone;
    }

    void CheckEnemyPool(const std::vector<BruteEnemy*> pool,
        const glm::vec3& ourPos,
        float& closestDist,
        Object*& closestEnemy)
    {
        for (BruteEnemy* enemy : pool) {
            if (!enemy->object->isActive) continue;

            float dist = glm::distance(ourPos, enemy->object->GetWorldPosition());
            if (dist < closestDist) {
                closestDist = dist;
                closestEnemy = enemy->object;
            }
        }
    }
};