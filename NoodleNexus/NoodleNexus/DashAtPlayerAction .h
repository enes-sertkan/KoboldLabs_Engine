#pragma once
#include "GOAPAction.h"
#include "aAgent.h"

class DashAtPlayerAction : public GOAPAction {
public:
    DashAtPlayerAction(float dashSpeed = 25.0f, float dashDistance = 20.0f, float prepTime = 0.5f)
        : dashSpeed(dashSpeed), dashDistance(dashDistance), preparationTime(prepTime) {
        preconditions["playerInRange"] = true;
        effects["playerDamaged"] = true;
        cost = 1.2f;
    }

    bool checkProceduralPrecondition(Agent* agent) override {
        glm::vec3 toPlayer = agent->maze->player->mesh->positionXYZ - agent->object->mesh->positionXYZ;
        float distance = glm::length(toPlayer);
        return true;
    }

    bool perform(Agent* agent, float deltaTime) override {
        switch (currentState) {
        case State::Preparing:
            return HandlePreparation(agent, deltaTime);
        case State::Dashing:
            return HandleDashing(agent, deltaTime);
        case State::Cooldown:
            return HandleCooldown(agent, deltaTime);
        }
        return false;
    }

    void reset() override {
        currentState = State::Preparing;
        dashDirection = glm::vec3(0);
        currentDashDistance = 0.0f;
        currentPrepTime = 0.0f;
        playerDamaged = false;
        currentCooldownTime = 0.0f;
      
    }

private:
    enum class State { Preparing, Dashing, Cooldown };

    float dashSpeed;
    float dashDistance;
    float preparationTime;
    State currentState = State::Preparing;
    glm::vec3 dashDirection;
    float currentDashDistance = 0.0f;
    float currentPrepTime = 0.0f;
    float currentCooldownTime = 0.0f;  // New: tracks cooldown elapsed time
    bool playerDamaged = false;

    bool HandlePreparation(Agent* agent, float deltaTime) {
       
        agent->worldState["stateLock"] = true;
        // Face the player
        glm::vec3 toPlayer = agent->maze->player->mesh->positionXYZ - agent->object->mesh->positionXYZ;
        dashDirection = glm::normalize(glm::vec3(toPlayer.x, 0.0f, toPlayer.z));
        agent->object->mesh->rotationEulerXYZ.y = glm::degrees(atan2(-dashDirection.x, -dashDirection.z));

        // Move back slightly
        glm::vec3 newPos = agent->object->mesh->positionXYZ - dashDirection * deltaTime;
        glm::vec3 checkPos = agent->object->mesh->positionXYZ - dashDirection * 1.f;
        if (CheckCollision(checkPos, agent->maze)) {
            agent->Damage(10);
            reset();
            agent->worldState["stateLock"] = false;
            return true;
        }
        agent->object->mesh->positionXYZ = newPos;

        // Transition to dashing after preparation time
        currentPrepTime += deltaTime;
        if (currentPrepTime >= preparationTime) {
            currentState = State::Dashing;
        }
        return false;
    }

    bool HandleDashing(Agent* agent, float deltaTime) {
        glm::vec3 movement = dashDirection * dashSpeed * deltaTime;
        glm::vec3 newPos = agent->object->mesh->positionXYZ + movement; 
        glm::vec3 checkPos = agent->object->mesh->positionXYZ - dashDirection * 2.f;
        agent->worldState["stateLock"] = true;
        // Collision check
        if (CheckCollision(checkPos, agent->maze)) {
            agent->Damage(20);
            reset();
            agent->worldState["stateLock"] = false;
            return true;
        }

        // Apply movement
        agent->object->mesh->positionXYZ = newPos;
        currentDashDistance += glm::length(movement);

        // Check for collision with the player
        if (glm::distance(agent->object->mesh->positionXYZ,
            agent->maze->player->mesh->positionXYZ) < agent->attackRange && !playerDamaged) {
            agent->maze->factory->playerCore->Damage(2);
            playerDamaged = true;
        }

        // If dash distance is reached, enter cooldown state
        if (currentDashDistance >= dashDistance) {
            currentState = State::Cooldown;
            agent->worldState["stateLock"] = false;
        }
        return false;
    }

    // New function to handle cooldown state. It waits for 2.5 seconds before resetting.
    bool HandleCooldown(Agent* agent, float deltaTime) {
        currentCooldownTime += deltaTime;
        if (currentCooldownTime >= 2.5f) {
            reset();
            agent->worldState["stateLock"] = false;
        }
        return false;
    }

    static bool CheckCollision(const glm::vec3& position, MazeGenerator* maze) {
        glm::vec3 checkPos = position;
        checkPos.x += maze->TILE_SIZE * 0.45f;
        checkPos.z += maze->TILE_SIZE * 0.6f;
        glm::vec2 mazePos = maze->WorldToGrid(checkPos);
        return maze->IsWall(mazePos.y, mazePos.x);
    }
};
