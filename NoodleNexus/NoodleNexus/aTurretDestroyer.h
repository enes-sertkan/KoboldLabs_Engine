#pragma once
#pragma once
#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/glm.hpp>
#include "cTurret.h"
class aTurretDestroyer : public Action {
public:
    LabAttackFactory* factory = nullptr;
    float destructionRange = 5.0f; // Max distance to destroy turrets

    void Update() override {
        if (!factory) return;

        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            // Find closest turret
            Turret* closestTurret = nullptr;
            float closestDistance = FLT_MAX;

            glm::vec3 playerPos = object->GetWorldPosition();

            for (Turret* turret : factory->turrets) {
                float dist = glm::distance(playerPos, turret->position);
                if (dist < destructionRange && dist < closestDistance) {
                    closestDistance = dist;
                    closestTurret = turret;
                }
            }

            // Destroy if found
            if (closestTurret) {
                closestTurret->Destroy();

                // Optional: Add destruction effect
                if (factory->scene) {
                   
                }
            }
        }
    }
};