#include "ShootAtPlayerAction.h"
#include "aAgent.h"

bool ShootAtPlayerAction::checkProceduralPrecondition(Agent* agent) {
    // Check if player is in attack range and visible
    glm::vec3 toPlayer = agent->maze->player->mesh->positionXYZ - agent->object->mesh->positionXYZ;
    float distance = glm::length(toPlayer);

    return (distance <= agent->attackRange);
       // && !agent->maze->IsWallBetween(agent->mazePosition, agent->maze->player->mazePosition);
}

bool ShootAtPlayerAction::perform(Agent* agent, float deltaTime) {
    // Handle cooldown
    currentCooldown -= deltaTime;
    if (currentCooldown > 0) return false; // Not ready to shoot yet

    // Shoot!
    agent->ShootAtPlayer(bulletSpeed);
    currentCooldown = cooldown; // Reset timer

    // Keep this action active (return false to continue next frame)
    return false;
}