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
            direction = glm::normalize(direction);
            agent->object->mesh->positionXYZ+= direction * agent->speed * deltaTime;
        }

        // Success if close enough to attack
        return (glm::distance(agent->object->mesh->positionXYZ, playerPos) <= agent->attackRange);
    }

   
    void reset() override {}
};