#pragma once

#include "Action.h"
#include "LabAttackFactory.h"
#include "aSoftBodyAction.hpp"
#include "aAgent.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include "BruteEnemy.h"
#include <Windows.h>
#include <minmax.h>

class SlimeController : public Action {
public:
    LabAttackFactory* factory = nullptr;
    SoftBody* softBody = nullptr;

    // All other slimes for avoidance
    std::vector<SoftBody*> otherSlimes;

    // Movement parameters
    float detectionRadius = 20.0f;
    float movementForce = 20.0f;

    // Avoidance parameters
    float avoidanceRadius = 3.0f;
    float avoidanceStrength = 5.0f;

    // Wobble parameters (when idle)
    float wobbleFrequency = 1.5f;   // cycles per second
    float wobbleAmplitude = 0.35f;  // upward force magnitude

    // Jump parameters
    float jumpTickInterval = 1.0f;   // seconds between jump checks
    float jumpRecoveryTime = 5.0f;   // cooldown after a jump
    float jumpDuration = 0.5f;   // how long jump force applies
    float jumpForceMagnitude = 3.0f;  // force strength
    float jumpMinDistance = 5.0f;   // min distance to enemy to consider jump
    float jumpMaxDistance = 10.0f;  // max distance to enemy
    float jumpChance = 0.9f;   // 10% chance

    // Damage parameters
    float damageRadius = 2.0f;
    float damageCooldown = 0.5f;
    float damageAmount = 1.0f;
    float timeSinceLastDamage = 0.0f;

    // Shake effect parameters
    float shakeRadius = 7.5f;
    float maxShakePower = 0.7f;
    float shakePowerSpeed = 1.0f;
    float currentShakePower = 0.0f;

    // Internal timers
    float wobbleTimer = 0.0f;
    float jumpTickTimer = 0.0f;
    float jumpTimer = 0.0f;
    float timeSinceLastJump = 1e6f;

    // Register another slime for avoidance
    void AddOtherSlime(SoftBody* sb) {
        if (sb && sb != softBody) {
            otherSlimes.push_back(sb);
        }
    }

    virtual void Update() override {
        if (!factory || !softBody || !softBody->object) return;

        float dt = object->scene->deltaTime;
        timeSinceLastDamage += dt;
        timeSinceLastJump += dt;
        jumpTickTimer += dt;
        softBody->softBody->UpdateGeometricCentrePoint();

        Agent* closestEnemy = FindClosestEnemy();
        UpdateShakeEffect(closestEnemy);

        // Jump logic tick
        if (jumpTickTimer >= jumpTickInterval) {
            jumpTickTimer -= jumpTickInterval;
            if (timeSinceLastJump >= jumpRecoveryTime && closestEnemy) {
                float dist = glm::distance(GetSlimeWorldPosition(), GetTargetWorldPosition(closestEnemy));
                if (dist >= jumpMinDistance && dist <= jumpMaxDistance) {
                    if ((static_cast<float>(rand()) / RAND_MAX) < jumpChance) {
                        jumpTimer = jumpDuration;
                        timeSinceLastJump = 0.0f;
                    }
                }
            }
        }

        bool moved = false;
        // If currently jumping, apply jump force
        if (jumpTimer > 0.0f) {
            jumpTimer -= dt;
            // direction: up + toward enemy if exists
            glm::vec3 dir(0.0f, 1.0f, 0.0f);
            if (closestEnemy) {
                glm::vec3 toEnemy = glm::normalize(GetTargetWorldPosition(closestEnemy) - GetSlimeWorldPosition());
                toEnemy.y = 0.3f; // bias upward
                dir = glm::normalize(glm::vec3(toEnemy.x, 1.5f, toEnemy.z));
            }
            softBody->ApplyForceAboveCenter(dir * jumpForceMagnitude);
            return;
        }

        // Regular movement if not jumping
        if (!closestEnemy) {
            moved = HandleMovementToPlayer();
        }
        else {
            moved = HandleMovement(closestEnemy);
            HandleDamage(closestEnemy);
        }

        // Wobble if idle and not moving
        if (!moved) {
            wobbleTimer += dt;
            float wave = std::sin(wobbleTimer * 2.0f * 3.14 * wobbleFrequency);
            float upForce = max(0.0f, wave) * wobbleAmplitude;
            softBody->ApplyForceAboveCenter(glm::vec3(0.0f, upForce, 0.0f));
        }
    }

private:
    void UpdateShakeEffect(Agent* target) {
        float dt = object->scene->deltaTime;
        if (!target) {
            currentShakePower = max(0.0f, currentShakePower - dt * shakePowerSpeed);
        }
        else {
            float dist = glm::distance(GetSlimeWorldPosition(), GetTargetWorldPosition(target));
            if (dist <= shakeRadius) {
                float norm = 1.0f - (dist / shakeRadius);
                float targetPower = norm * maxShakePower;
                currentShakePower = glm::mix(currentShakePower, targetPower, dt * shakePowerSpeed);
            }
            else {
                currentShakePower = max(0.0f, currentShakePower - dt * shakePowerSpeed);
            }
        }
        softBody->object->mesh->shakePower = currentShakePower;
    }

    // Returns true if a movement force was applied
    bool HandleMovement(Agent* target) {
        glm::vec3 pos = GetSlimeWorldPosition();
        glm::vec3 tgt = GetTargetWorldPosition(target);
        glm::vec3 dir(0.0f);

        if (glm::distance(pos, tgt) > 0.1f) {
            dir = glm::normalize(tgt - pos);
            dir.y = 0.0f;
        }

        // Avoid other slimes
        glm::vec3 avoid(0.0f);
        for (SoftBody* other : otherSlimes) {
            if (!other || other == softBody) continue;
            glm::vec3 op = other->softBody->getGeometricCentrePoint() *
                other->object->GetWorldScale() + other->object->GetWorldPosition();
            glm::vec3 d = pos - op;
            float dist = glm::length(d);
            if (dist < avoidanceRadius && dist > 0.0f) {
                avoid += glm::normalize(d) * ((avoidanceRadius - dist) / avoidanceRadius);
            }
        }
        if (glm::length(avoid) > 0.0f) {
            avoid = glm::normalize(avoid) * avoidanceStrength;
            dir += avoid;
        }

        if (glm::length2(dir) > 0.0f) {
            dir = glm::normalize(dir);
            ApplyMovementForce(dir);
            return true;
        }
        return false;
    }

    bool HandleMovementToPlayer() {
        glm::vec3 pos = GetSlimeWorldPosition();
        glm::vec3 playerPos = factory->playerCore->object->GetWorldPosition();
        glm::vec3 dir(0.0f);

        float distToPlayer = glm::distance(pos, playerPos);
        if (distToPlayer > 4.0f && distToPlayer < detectionRadius) {
            dir = glm::normalize(playerPos - pos);
            dir.y = 0.0f;
        }

        // Avoid other slimes
        glm::vec3 avoid(0.0f);
        for (SoftBody* other : otherSlimes) {
            if (!other || other == softBody) continue;
            glm::vec3 op = other->object->mesh->positionXYZ;
            glm::vec3 d = pos - op;
            float dist = glm::length(d);
            if (dist < avoidanceRadius && dist > 0.0f) {
                avoid += glm::normalize(d) * ((avoidanceRadius - dist) / avoidanceRadius);
            }
        }
        if (glm::length(avoid) > 0.0f) {
            avoid = glm::normalize(avoid) * avoidanceStrength;
            dir += avoid;
        }

        if (glm::length2(dir) > 0.0f) {
            dir = glm::normalize(dir);
            ApplyMovementForce(dir);
            return true;
        }
        return false;
    }

    void HandleDamage(Agent* target) {
        glm::vec3 pos = GetSlimeWorldPosition();
        glm::vec3 tgt = GetTargetWorldPosition(target);
        if (glm::distance(pos, tgt) <= damageRadius &&
            timeSinceLastDamage >= damageCooldown) {
            ApplyDamage(target);
            timeSinceLastDamage = 0.0f;
            currentShakePower = min(maxShakePower, currentShakePower + 0.1f);
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
        glm::vec3 pos = GetSlimeWorldPosition();
        float best = FLT_MAX;
        Agent* closest = nullptr;
        for (BruteEnemy* e : factory->m_creepPool) {
            if (!e->object->isActive) continue;
            float d = glm::distance(pos, GetTargetWorldPosition(e));
            if (d < detectionRadius && d < best) {
                best = d;
                closest = e;
            }
        }
        return closest;
    }

    void ApplyMovementForce(const glm::vec3& direction) {
        glm::vec3 force = direction * movementForce * object->scene->deltaTime;
        force.y = 0.0f;
        softBody->ApplyForceAboveCenter(force);
    }
};
