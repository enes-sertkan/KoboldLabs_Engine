#pragma once
#include "GOAPAction.h"
#include "aAgent.h"

class MoveToPlayerAction : public GOAPAction {
public:
    MoveToPlayerAction() {
        preconditions["playerVisible"] = true;
        effects["playerInRange"] = true;
        cost = 1.0f;
    }

    bool checkProceduralPrecondition(Agent* agent) override {
        return true; // Can always try to move
    }

    bool perform(Agent* agent, float deltaTime) override {
        glm::vec3 playerPos = agent->maze->player->mesh->positionXYZ;
        glm::vec3 direction = playerPos - agent->object->mesh->positionXYZ;

        if (glm::length(direction) > 0) {
            direction = glm::normalize(direction);
            agent->object->mesh->positionXYZ+= direction * agent->speed * deltaTime;
        }

        // Success if close enough to attack
        return (glm::distance(agent->mazePosition, playerPos) <= agent->attackRange);
    }

    bool isDone() const override { return false; } // Handled in perform()
    void reset() override {}
};