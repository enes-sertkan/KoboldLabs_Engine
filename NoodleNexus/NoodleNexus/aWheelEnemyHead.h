#pragma once
#include "Action.h"
#include "glm/glm.hpp"
#include "sObject.h"
#include "Scene.hpp"
#include "aAgent.h"

class aWheelEnemyHead : public Action {
private:
    float recoilProgress = 0.0f;
    float previousHealth = 0.0f;

    // Configuration
    const glm::vec2 pitchRange = glm::vec2(10.0f, -30.0f); // Min/max angles (in degrees)
    const float recoilAngle = 35.0f;        // Maximum angular recoil offset (degrees)
    const float recoilSpeed = 3.0f;         // Recovery speed for the recoil animation

public:
    Agent* agent = nullptr;

    virtual void Start() override {
        if (agent) {
            // Initialize previousHealth to the agent's starting health
            previousHealth = agent->health;
        }
    }

    virtual void Update() override {
        if (!agent)
            return;

        // Check if health has dropped since last frame
        if (agent->health < previousHealth) {
            TriggerRecoil();
        }
        // Update previousHealth for next frame comparison
        previousHealth = agent->health;

        // 1. Health-based pitch rotation
        float healthRatio = agent->health / agent->maxHealth;
        float targetPitch = glm::mix(pitchRange.x, pitchRange.y, 1.0f - healthRatio);

        // 2. Handle recoil animation: Calculate angular offset
        float recoilOffset = 0.0f;
        if (recoilProgress > 0.0f) {
            // Using a sine curve for a smooth recoil bump effect.
            recoilOffset = sin(recoilProgress * glm::pi<float>()) * recoilAngle;
            recoilProgress -= object->scene->deltaTime * recoilSpeed;
            if (recoilProgress < 0.0f)
                recoilProgress = 0.0f;
        }

        // Apply the combined rotation (health-based pitch + recoil offset)
        object->mesh->rotationEulerXYZ.x = targetPitch + recoilOffset;
    }

    void TriggerRecoil() {
        recoilProgress = 1.0f; // Restart recoil animation.
    }
};
