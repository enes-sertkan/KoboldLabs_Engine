#pragma once
#include "aParticleEmitter .h"

class aTargetedParticleEmitter : public aParticleEmitter {
public:
    std::vector<glm::vec3> particleTargets; // Stores target position for each particle
    glm::vec3 emitterStartPos;
    glm::vec3 emitterTargetPos;
    float arrivalThreshold = 0.4f;

    glm::vec4 colorStart = glm::vec4(0.4, 0.4, 0.4, 0.8);
    glm::vec4  colorEnd = glm::vec4(0.6, 0.6, 0.6, 0);
    float particleSpeed = 12.0f;
    bool useRandomStartOffset = false;
    glm::vec3 startOffsetRange = glm::vec3(0.5f);
    bool useRandomTargetOffset = false;
    glm::vec3 targetOffsetRange = glm::vec3(0.5f);


    virtual void Start() override {
        aParticleEmitter::Start();
        particleTargets.resize(maxParticles);
        damping = glm::vec3(1.0f); // Disable velocity damping
    }

    virtual void Update() override {
        float deltaTime = object->scene->deltaTime;

        // Update existing particles
        if (!firstSpawn) {
            int activeCount = 0;
            for (int i = 0; i < particles->size(); i++) {
                if (!particles->at(i).active) continue;

                // Calculate direction to target
                glm::vec3 toTarget = particleTargets[i] - particles->at(i).position;
                float distanceToTarget = glm::length(toTarget);

                // Update velocity to always point at target
                if (distanceToTarget > arrivalThreshold) {
                    glm::vec3 direction = glm::normalize(toTarget);
                    particles->at(i).velocity = direction * particleSpeed;

                    // Update position and rotation
                    particles->at(i).position += particles->at(i).velocity * 0.05f;
                    particles->at(i).rotation += particles->at(i).rotationSpeed * deltaTime;

                    // Update visual properties
                    float lifeRatio = particles->at(i).lifeRemaining / particles->at(i).lifetime;
                    particles->at(i).color = glm::mix(colorEnd, colorStart, lifeRatio);
                    particles->at(i).size = glm::mix(sizeEnd, sizeStart, lifeRatio);

                    activeCount++;
                }
                else {
                    particles->at(i).active = false;
                }
            }

            if (activeCount <= 0 && !spawnActive && destroyOnNoParticles) {
                object->Destroy();
            }
        }

        // Spawn new particles
        particlesToSpawn += spawnRate * deltaTime;
        if (spawnActive || firstSpawn) {
            for (int i = 0; i < particlesToSpawn; i++) {
                int id = GetNextAvailableParticle();
                if (id == -1) break;

                // Calculate start/target positions with optional randomization
                glm::vec3 start = useRandomStartOffset ?
                    emitterStartPos + glm::vec3(
                        randomFloat(-startOffsetRange.x, startOffsetRange.x),
                        randomFloat(-startOffsetRange.y, startOffsetRange.y),
                        randomFloat(-startOffsetRange.z, startOffsetRange.z)
                    ) : object->GetWorldPosition();

                glm::vec3 target = useRandomTargetOffset ?
                    emitterTargetPos + glm::vec3(
                        randomFloat(-targetOffsetRange.x, targetOffsetRange.x),
                        randomFloat(-targetOffsetRange.y, targetOffsetRange.y),
                        randomFloat(-targetOffsetRange.z, targetOffsetRange.z)
                    ) : emitterTargetPos;

                // Initialize particle
                particles->at(id).position = start;
                particles->at(id).velocity = glm::vec3(0); // Direction will be calculated in Update
                particles->at(id).lifetime = particles->at(id).lifeRemaining =
                    glm::distance(start, target) / particleSpeed;
                particles->at(id).color = colorStart;
                particles->at(id).size = sizeStart;
                particles->at(id).rotation = randomRotationSpeed();
                particles->at(id).rotationSpeed = randomRotationSpeed();
                particles->at(id).active = true;

                // Store target position
                particleTargets[id] = target;
            }
            particlesToSpawn -= floorf(particlesToSpawn);
        }

        firstSpawn = false;
        object->mesh->pParticles = particles;
    }

private:
    int GetNextAvailableParticle()  {
        for (int i = 0; i < particles->size(); i++) {
            if (!particles->at(i).active) return i;
        }
        return -1;
    }
};