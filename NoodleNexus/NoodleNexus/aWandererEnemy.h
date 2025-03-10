#pragma once
#include "aLACharacter.h"
#include <vector>
#include <chrono>

class aWandererEnemy : public aLACharacter {
public:
    std::vector<glm::vec3> wanderPoints; // List of points to wander between
    int currentWanderIndex = 0;          // Current target point index
    float wanderDuration = 6.0f;         // Time to spend wandering (in seconds)
    float idleDuration = 3.0f;           // Time to spend idling (in seconds)
    float closeEnoughDistance = 5.0f;    // Distance threshold to consider a point "reached"

    float stateTimer = 0.0f;             // Timer to track state duration

    void Start() override {
     
        aLACharacter::Start();
        // Initialize wander points (you can set these in the scene or hardcode them)
        moveSpeed = 100.f;
        wanderPoints = {
            glm::vec3(420.f, object->mesh->positionXYZ.y, 50.0f),
            glm::vec3(150.0f, object->mesh->positionXYZ.y, 40.0f),
            glm::vec3(150.0f, object->mesh->positionXYZ.y, 0.0f),
            glm::vec3(420.0f, object->mesh->positionXYZ.y, 0.0f)
        };

        // Start in the WanderingState
        stateMachine->ChangeState(std::bind(&aWandererEnemy::WanderingState, this, std::placeholders::_1));
    }

    // State: Wander between points
    void WanderingState(StateMachine* stateMachine) {
        stateTimer += object->scene->deltaTime;

        if (stateTimer >= wanderDuration) {
            // Transition to IdleState after wandering for given seconds
            stateTimer = 0.0f;
            stateMachine->ChangeState(std::bind(&aWandererEnemy::IdleState, this, std::placeholders::_1));
            return;
        }

        // Move towards the current wander point
        glm::vec3 targetPoint = wanderPoints[currentWanderIndex];
        MoveTowards(targetPoint);

        // Check if close enough to the current point
        float distanceToPoint = glm::length(targetPoint - object->mesh->positionXYZ);
        if (distanceToPoint < closeEnoughDistance) {
            // Move to the next point in the list
            currentWanderIndex = (currentWanderIndex + 1) % wanderPoints.size();
        }
    }

    // State: Idle (do nothing)
    void IdleState(StateMachine* stateMachine) {
        stateTimer += object->scene->deltaTime;

        if (stateTimer >= idleDuration) {
            // Transition back to WanderingState after idling for 3 seconds
            stateTimer = 0.0f;
            stateMachine->ChangeState(std::bind(&aWandererEnemy::WanderingState, this, std::placeholders::_1));
        }
    }
};