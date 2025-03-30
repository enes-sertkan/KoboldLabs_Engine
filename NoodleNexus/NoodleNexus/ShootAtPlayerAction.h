#pragma once
#include "GOAPAction.h"

class ShootAtPlayerAction : public GOAPAction {
public:
    ShootAtPlayerAction(float cooldown = 1.0f, float bulletSpeed = 15.0f)
        : cooldown(cooldown), bulletSpeed(bulletSpeed) {
        // Preconditions
        preconditions["playerInRange"] = true;

        // Effects
        effects["playerDamaged"] = true;

        cost = 0.8f; // Medium priority
    }

    bool checkProceduralPrecondition(Agent* agent) override;
    bool perform(Agent* agent, float deltaTime) override;
  
    void reset() override { currentCooldown = 0.0f; }

private:
    float cooldown=1.f;
    float bulletSpeed = 3.f;
    float currentCooldown = 0.0f;
};