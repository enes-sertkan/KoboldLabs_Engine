#pragma once
#pragma once
#include "Action.h"
#include "LabAttackFactory.h"
#include <glm/glm.hpp>
#include "cTurret.h"
#include "aTargetedParticleEmitter .h"

class aTurretDestroyer : public Action {
public:
    LabAttackFactory* factory = nullptr;
    float destructionRange = 10.0f; // Max distance to destroy turrets
    aTargetedParticleEmitter* particles = nullptr;
    Turret* targetTurret = nullptr;
    void Update() override {
        if (particles)
            particles->spawnActive = false;

        if (!factory) return;
 
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
            if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
                targetTurret = closestTurret;


            // Destroy if found
            if (targetTurret) {
                particles->spawnActive = true;
                particles->emitterTargetPos = targetTurret->neck->headConnection->GetWorldPosition();
             

             if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                 particles->SetAllActiveToReturning();
                 targetTurret->Destroy();
                 targetTurret = nullptr;
                // Optional: Add destruction effect
                if (factory->scene) {
                   
                }
            }
        }
    }
};