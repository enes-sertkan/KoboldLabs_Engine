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
        glm::vec3 currentPos = agent->object->mesh->positionXYZ;
        // Ensure Y level is the same for movement
        playerPos.y = currentPos.y;

        glm::vec3 direction = playerPos - currentPos;
        if (glm::length(direction) > 0.0f) {
            direction.y = 0;
            direction = glm::normalize(direction);

            // Compute the new position using agent speed
            glm::vec3 newPos = currentPos + direction * agent->speed * deltaTime;
            // Check collision at the new position
            if (CheckCollision(newPos, agent->maze)) {
                // Colliding; inflict some damage and skip moving this frame
                agent->Damage(5);
                return false;
            }
            // No collision: update position
            agent->object->mesh->positionXYZ = newPos;

            // Update rotation to face player smoothly
            float targetYaw = glm::degrees(glm::atan(-direction.x, -direction.z));
            float currentYaw = agent->object->mesh->rotationEulerXYZ.y;
            float deltaYaw = targetYaw - currentYaw;
            if (deltaYaw > 180.0f) deltaYaw -= 360.0f;
            if (deltaYaw < -180.0f) deltaYaw += 360.0f;
            float rotationSpeed = 200.0f; // degrees per second
            float maxRotationStep = rotationSpeed * deltaTime;
            float rotationStep = glm::clamp(deltaYaw, -maxRotationStep, maxRotationStep);
            agent->object->mesh->rotationEulerXYZ.y += rotationStep;
        }

        // Return success if close enough to attack the player
        return (glm::distance(agent->object->mesh->positionXYZ, playerPos) <= agent->attackRange - 0.5f);
    }

    void reset() override {}

private:
    // Helper collision check function using a similar system as in DashAtPlayerAction.
    static bool CheckCollision(const glm::vec3& position, MazeGenerator* maze) {
        // Adjust the position slightly to account for the object's bounds.
        glm::vec3 checkPos = position;
        checkPos.x += maze->TILE_SIZE * 0.45f;
        checkPos.z += maze->TILE_SIZE * 0.6f;
        glm::vec2 mazePos = maze->WorldToGrid(checkPos);
        return maze->IsWall(mazePos.y, mazePos.x);
    }
};
