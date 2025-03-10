#pragma once
#include "aLACharacter.h"

class aShooterEnemy : public aLACharacter {
public:
    float minDistance = 50.0f;  // Minimum distance to maintain from the player
    float maxDistance = 150.0f; // Maximum distance to maintain from the player

    void Start() override {
        aLACharacter::Start();
        // Initial state set to MovingState
        stateMachine->ChangeState(std::bind(&aShooterEnemy::MovingState, this, std::placeholders::_1));
    }

    // State: Moving to maintain the radius
    void MovingState(StateMachine* stateMachine) {
        float distanceToPlayer = glm::length(player->mesh->positionXYZ - object->mesh->positionXYZ);

        if (distanceToPlayer > maxDistance) {
            // Move closer to the player
            MoveTowards(player->mesh->positionXYZ);
        }
        else if (distanceToPlayer < minDistance) {
            // Move away from the player
            MoveAwayFrom(player->mesh->positionXYZ);
        }
        else {
            // Within the desired radius, transition to ShootingState
            stateMachine->ChangeState(std::bind(&aShooterEnemy::ShootingState, this, std::placeholders::_1));
        }
    }

    // State: Shooting at the player
    void ShootingState(StateMachine* stateMachine) {
        float distanceToPlayer = glm::length(player->mesh->positionXYZ - object->mesh->positionXYZ);

        if (distanceToPlayer < minDistance || distanceToPlayer > maxDistance) {
            // Outside the desired radius, transition back to MovingState
            stateMachine->ChangeState(std::bind(&aShooterEnemy::MovingState, this, std::placeholders::_1));
            return;
        }

        // Shoot at the player
        ShootAtPlayer(30.0f); // Adjust bullet speed as needed
    }
}; 
