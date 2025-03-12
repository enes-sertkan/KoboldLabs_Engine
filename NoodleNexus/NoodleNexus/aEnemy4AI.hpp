#pragma once
#include "aEnemyAI.hpp"

class Enemy4AI : public aEnemyAI {
private:
    float wanderTime = 6.0f;
    float idleTime = 3.0f;
    float stateTimer = 0.0f;
    bool isWandering = true;
    std::vector<glm::vec3> patrolPoints;
    size_t currentPatrolIndex = 0;

public:
    void Start() override {
        aEnemyAI::Start();
        InitializePatrolPath();
        stateTimer = wanderTime;
    }

    void Update() override {
        stateTimer -= object->scene->deltaTime;

        if (isWandering) {
            Wander();
            if (stateTimer <= 0) SwitchState(false);
        }
        else {
            Idle();
            if (stateTimer <= 0) SwitchState(true);
        }
    }

    void InitializePatrolPath() {
        // Define a simple square patrol path
        patrolPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));   // Point 1
        patrolPoints.push_back(glm::vec3(5.0f, 0.0f, 0.0f));   // Point 2
        patrolPoints.push_back(glm::vec3(5.0f, 0.0f, 5.0f));   // Point 3
        patrolPoints.push_back(glm::vec3(0.0f, 0.0f, 5.0f));   // Point 4
    }

    void Wander() {
        if (patrolPoints.empty()) return;

        // Get the current target patrol point
        glm::vec3 targetPoint = patrolPoints[currentPatrolIndex];

        // Move towards the target point (ignore Y-axis for movement)
        glm::vec3 direction = glm::normalize(glm::vec3(targetPoint.x, 0.0f, targetPoint.z) -
            glm::vec3(object->mesh->positionXYZ.x, 0.0f, object->mesh->positionXYZ.z));
        float moveSpeed = 3.0f; // Speed of movement
        object->mesh->positionXYZ += direction * moveSpeed * object->scene->deltaTime;

        // Rotate the enemy to face the target point while moving
        RotateTowards(targetPoint);

        // Check if the enemy has reached the target point
        if (glm::distance(glm::vec3(object->mesh->positionXYZ.x, 0.0f, object->mesh->positionXYZ.z),
            glm::vec3(targetPoint.x, 0.0f, targetPoint.z)) < 0.5f) {
            currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
        }
    }

    void Idle() {
        // Idle logic (no movement or rotation)
        return;
    }

    void SwitchState(bool newWanderState) {
        isWandering = newWanderState;
        stateTimer = isWandering ? wanderTime : idleTime;
    }

private:
    void RotateTowards(const glm::vec3& target) {
        // Calculate the direction towards the target (ignore Y-axis for rotation)
        glm::vec3 direction = glm::normalize(glm::vec3(target.x, 0.0f, target.z) -
            glm::vec3(object->mesh->positionXYZ.x, 0.0f, object->mesh->positionXYZ.z));

        // Calculate the angle in radians (XZ plane)
        float angle = atan2(direction.x, direction.z);

        // Convert radians to degrees (if your rotation system uses degrees)
        float angleDegrees = glm::degrees(angle);

        // Update the rotation Euler angles (yaw is around the Y-axis)
        object->mesh->rotationEulerXYZ.y = angleDegrees;
    }
};