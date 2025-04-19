#pragma once
#include "Action.h"
#include "LabAttackFactory.h"
#include "aSoftBodyAction.hpp"
#include "aAgent.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "BruteEnemy.h"
#include <Windows.h>
#include <minmax.h>

class SlimeController : public Action {
public:
    LabAttackFactory* factory = nullptr;
    SoftBody* softBody = nullptr;

    // Movement parameters
    float detectionRadius = 20.0f;
    float movementForce = 20.0f;

    // Damage parameters
    float damageRadius = 2.0f;
    float damageCooldown = 0.5f;
    float damageAmount = 1.0f;
    float timeSinceLastDamage = 0.0f;

    // Shake effect parameters
    float shakeRadius = 7.5f;          // Distance where shaking starts
    float maxShakePower = 0.7f;        // Maximum intensity of shaking
    float shakePowerSpeed = 1.0f;      // How quickly shake power increases
    float currentShakePower = 0.0f;    // Current shake intensity

    virtual void Update() override {
        if (!factory || !softBody || !softBody->object) return;

        timeSinceLastDamage += object->scene->deltaTime;
        softBody->softBody->UpdateGeometricCentrePoint();

        // Find closest enemy
        Agent* closestEnemy = FindClosestEnemy();
        Agent* currentTarget = closestEnemy;

        // Update shake effect based on proximity
        UpdateShakeEffect(closestEnemy);

        if (!currentTarget) {
            HandleMovementToPlayer();
        }
        else {
            HandleMovement(currentTarget);
            HandleDamage(currentTarget);
        }
    }

private:
    void UpdateShakeEffect(Agent* target) {
        if (!target) {
            // No target - smoothly reduce shake
            currentShakePower = max(0.0f, currentShakePower - object->scene->deltaTime * shakePowerSpeed);
        }
        else {
            // Calculate distance to target
            float distance = glm::distance(GetSlimeWorldPosition(), GetTargetWorldPosition(target));

            // If within shake radius, calculate shake power
            if (distance <= shakeRadius) {
                // Normalized distance (1.0 at contact, 0.0 at shakeRadius)
                float normalizedDist = 1.0f - (distance / shakeRadius);
                // Target shake power based on distance
                float targetShakePower = normalizedDist * maxShakePower;
                // Smoothly interpolate current shake power
                currentShakePower = glm::mix(currentShakePower, targetShakePower,
                    object->scene->deltaTime * shakePowerSpeed);
            }   
            else {
                // Outside shake radius - reduce shake
                currentShakePower = max(0.0f, currentShakePower - object->scene->deltaTime * shakePowerSpeed);
            }
        }

        // Apply shake power to mesh
        softBody->object->mesh->shakePower = currentShakePower;
    }

    void HandleMovement(Agent* target) {
        glm::vec3 slimePos = GetSlimeWorldPosition();
        glm::vec3 targetPos = GetTargetWorldPosition(target);

        if (glm::distance(slimePos, targetPos) > 0.1f) {
            glm::vec3 forceDir = glm::normalize(targetPos - slimePos);
            forceDir.y = 0;
            forceDir = glm::normalize(forceDir);
            ApplyMovementForce(forceDir);
        }
    }

    void HandleMovementToPlayer() {
        glm::vec3 slimePos = GetSlimeWorldPosition();
        glm::vec3 targetPos = factory->playerCore->object->GetWorldPosition();

        if (glm::distance(slimePos, targetPos) > 4.0f) {
            glm::vec3 forceDir = glm::normalize(targetPos - slimePos);
            forceDir.y = 0;
            forceDir = glm::normalize(forceDir);
            ApplyMovementForce(forceDir);
        }
    }

    void HandleDamage(Agent* target) {
        glm::vec3 slimePos = GetSlimeWorldPosition();
        glm::vec3 targetPos = GetTargetWorldPosition(target);

        if (glm::distance(slimePos, targetPos) <= damageRadius) {
            if (timeSinceLastDamage >= damageCooldown) {
                ApplyDamage(target);
                timeSinceLastDamage = 0.0f;
                // Add extra shake when dealing damage
                currentShakePower = min(maxShakePower, currentShakePower + 0.1f);
            }
        }
    }

    void ApplyDamage(Agent* target) {
        if (BruteEnemy* enemy = dynamic_cast<BruteEnemy*>(target)) {
            enemy->Damage(damageAmount);
        }
   
    }

    glm::vec3 GetSlimeWorldPosition() const {
        return softBody->softBody->getGeometricCentrePoint() *
            softBody->object->GetWorldScale() +
            softBody->object->GetWorldPosition();
    }

    glm::vec3 GetTargetWorldPosition(Agent* target) const {
        return target->object->GetWorldPosition() + target->colliderCenter;
    }

    Agent* FindClosestEnemy() {
        if (!factory) return nullptr;

        glm::vec3 slimePos = GetSlimeWorldPosition();
        float closestDist = FLT_MAX;
        Agent* closestEnemy = nullptr;

        for (BruteEnemy* enemy : factory->m_creepPool) {
            if (!enemy->object->isActive) continue;

            float dist = glm::distance(slimePos, GetTargetWorldPosition(enemy));
            if (dist < detectionRadius && dist < closestDist) {
                closestDist = dist;
                closestEnemy = enemy;
            }
        }

        return closestEnemy;
    }

    void ApplyMovementForce(const glm::vec3& direction) {
        glm::vec3 force = direction * movementForce * object->scene->deltaTime;
        softBody->ApplyForceAboveCenter(force);
    }
};