#pragma once
#include "aParticleEmitter .h"

class aTargetedParticleEmitter : public aParticleEmitter {
public:
    std::vector<glm::vec3> particleTargets;
    glm::vec3 emitterStartPos;
    glm::vec3 emitterTargetPos;
    float arrivalThreshold = 0.4f;
    float particleSpeed = 8.0f;

    // Progression control
    float subspeed = 0.4f;         // Speed of spawn point movement towards target
    bool bLoop = true;             // Loop the spawn point movement
    float pathProgress = 0.0f;     // Progress along the path from start to target

    virtual void Start() override {
        aParticleEmitter::Start();
        particleTargets.resize(maxParticles);
        damping = glm::vec3(1.0f);
        emitterStartPos = object->GetWorldPosition();
        pathProgress = 0.0f; // Initialize progress
    }

    virtual void Update() override {
        emitterStartPos = object->GetWorldPosition();
        float deltaTime = object->scene->deltaTime;

        // Calculate path from current position to target
        glm::vec3 toTarget = emitterTargetPos - emitterStartPos;
        float totalDistance = particleSpeed * 0.05F;//glm::length(toTarget);

    

        // Current spawn position along the path
        glm::vec3 currentSpawnPos = emitterStartPos + glm::normalize(toTarget) * totalDistance * pathProgress;

        // Update existing particles
        if (!firstSpawn) {
            int activeCount = 0;


            for (int i = 0; i < particles->size(); i++) {
                if (!particles->at(i).active) continue;

                // Update target for returning particles
                if (particles->at(i).returning) {
                    particleTargets[i] = emitterStartPos;
                }
                glm::vec3 toTarget = particleTargets[i] - particles->at(i).position;
                float distanceToTarget = glm::length(toTarget);

                if (distanceToTarget > arrivalThreshold) {
                    glm::vec3 moveDir = glm::normalize(toTarget);
                    particles->at(i).position += moveDir * particleSpeed * 0.05f;
                    particles->at(i).rotation += particles->at(i).rotationSpeed * deltaTime;

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

        // Spawn new particles at current spawn position
        particlesToSpawn = 1;
        if (spawnActive || firstSpawn) {
            for (int i = 0; i < particlesToSpawn; i++) {
                int id = GetNextAvailableParticle();
                if (id == -1) break;

                glm::vec3 target = emitterTargetPos;
                float remainingDistance = glm::distance(currentSpawnPos, target);

                particles->at(id).position = currentSpawnPos;
                particles->at(id).lifetime = particles->at(id).lifeRemaining = remainingDistance / particleSpeed;
                particles->at(id).color = colorStart;
                particles->at(id).size = sizeStart;
                particles->at(id).rotation = randomRotationSpeed();
                particles->at(id).rotationSpeed = randomRotationSpeed();
                particles->at(id).active = true;
                particles->at(id).returning = false;

                particleTargets[id] = target;


                // Update spawn position progression
                if (totalDistance > 0.0f) {
                    pathProgress += (subspeed * deltaTime) / totalDistance;
                   
                    if (pathProgress >= 1)
                    {
                        particlesToSpawn++;
                        pathProgress = 0;
                        currentSpawnPos = emitterStartPos + glm::normalize(toTarget) * totalDistance * pathProgress;
                    }


                      

                
                }
            }
        }

        firstSpawn = false;
        object->mesh->pParticles = particles;
    }
    void SetAllActiveToReturning() {
        for (int i = 0; i < particles->size(); i++) {
            if (particles->at(i).active) {
                particles->at(i).returning = true;
                // Optionally update target immediately
                particleTargets[i] = emitterStartPos;
            }
        }
    }
private:
    int GetNextAvailableParticle() {
        for (int i = 0; i < particles->size(); i++) {
            if (!particles->at(i).active) return i;
        }
        return -1;
    }
};