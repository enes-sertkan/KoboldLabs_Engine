#pragma once
#include "GOAPAction.h"
#include "aAgent.h"

class MoveToPlayerAction : public GOAPAction {
public:
    MoveToPlayerAction() {
        preconditions["playerVisible"] = true;
        effects["playerInRange"] = true;
        cost = 0.1f;
    }

    bool checkProceduralPrecondition(Agent* agent) override {
        return true; // Can always try to move
    }

    bool perform(Agent* agent, float deltaTime) override {
        glm::vec3 playerPos = agent->maze->player->mesh->positionXYZ;
        glm::vec3 direction = playerPos - agent->object->mesh->positionXYZ;

        //Just a stupid way to make it move on flat surface
        playerPos.y = agent->object->mesh->positionXYZ.y;

        if (glm::length(direction) > 0) {
            direction.y = 0;
            direction = glm::normalize(direction);
            agent->object->mesh->positionXYZ+= direction * agent->speed * deltaTime;



            float targetYaw = glm::degrees(glm::atan(-direction.x, -direction.z));

            // Get current rotation
            float currentYaw = agent->object->mesh->rotationEulerXYZ.y;

            // Calculate shortest angle difference
            float deltaYaw = targetYaw - currentYaw;
            if (deltaYaw > 180) deltaYaw -= 360;
            if (deltaYaw < -180) deltaYaw += 360;

            // Smooth rotation interpolation
            float rotationSpeed = 200.0f; // Degrees per second
            float maxRotationStep = rotationSpeed * agent->object->scene->deltaTime;
            float rotationStep = glm::clamp(deltaYaw, -maxRotationStep, maxRotationStep);

            // Apply rotation
            agent->object->mesh->rotationEulerXYZ.y += rotationStep;
        }

        // Success if close enough to attack
        return (glm::distance(agent->object->mesh->positionXYZ, playerPos) <= agent->attackRange-0.5f);
    }

   
    void reset() override {}
};