#pragma once
#include "aLACharacter.h"

class aAvoiderEnemy : public aLACharacter {
public:
    float avoidRadius = 60.0f; // Radius within which to avoid bullets
    Object* closestBullet = nullptr; // Track the closest bullet to avoid

    void Start() override {
        aLACharacter::Start();
        // Initial state set to PursueState
     
        stateMachine->ChangeState(std::bind(&aAvoiderEnemy::PursueState, this, std::placeholders::_1));
    }

    // Helper function to find the closest bullet within avoidRadius
    Object* GetCloseBullet() {
        float closestDistance = avoidRadius;
        Object* closest = nullptr;

        for (Object* bullet : factory->m_playerBulletPool) {
            if (!bullet->isActive) continue; // Skip inactive bullets

            float distance = glm::length(bullet->mesh->positionXYZ - object->mesh->positionXYZ);
            if (distance < closestDistance) {
                closestDistance = distance;
                closest = bullet;
            }
        }

        return closest;
    }

    // State: Pursue the player
    void PursueState(StateMachine* stateMachine) {
        closestBullet = GetCloseBullet();

        if (closestBullet) {
            // If a bullet is close, transition to AvoidState
            stateMachine->ChangeState(std::bind(&aAvoiderEnemy::AvoidState, this, std::placeholders::_1));
            return;
        }
        moveSpeed = 12.f;
        // Otherwise, pursue the player
        MoveTowards(player->mesh->positionXYZ);
    }

    // State: Avoid the closest bullet
    void AvoidState(StateMachine* stateMachine) {
        if (!closestBullet || !closestBullet->isActive) {
            // If the bullet is no longer active or out of range, transition back to PursueState
            closestBullet = nullptr;
            stateMachine->ChangeState(std::bind(&aAvoiderEnemy::PursueState, this, std::placeholders::_1));
            return;
        }
        moveSpeed = 50.f;
        // Move away from the closest bullet
        MoveAwayFrom(closestBullet->mesh->positionXYZ);

        // Check if the bullet is still close
        float distanceToBullet = glm::length(closestBullet->mesh->positionXYZ - object->mesh->positionXYZ);
        if (distanceToBullet > avoidRadius) {
            // If the bullet is no longer close, transition back to PursueState
            closestBullet = nullptr;
            stateMachine->ChangeState(std::bind(&aAvoiderEnemy::PursueState, this, std::placeholders::_1));
        }
    }
};