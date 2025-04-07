#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <GLFW/glfw3.h>


struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float size;
    float lifetime;
    float lifeRemaining;
    bool active = false;
};

class aParticleEmitter : public Action {
private:
    std::vector<Particle> particles;
    GLuint instanceVBO;
    unsigned int maxParticles = 1000;
    float spawnRate = 50.0f; // Particles per second

    // Particle properties
    glm::vec3 emitterPosition;
    glm::vec3 velocityRange[2];
    glm::vec4 colorStart, colorEnd;
    float sizeStart, sizeEnd;

public:
    virtual void Start() override {
        particles.resize(maxParticles);
        SetupInstancing();
    }

    void SetupInstancing() {
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);

        // Attach to VAO (using your existing VAO manager)
        sModelDrawInfo meshInfo;
        if (object->scene->vaoManager->FindDrawInfoByModelName("assets/models/Sphere_radius_1_xyz_N_uv.ply", meshInfo)) {
            glBindVertexArray(meshInfo.VAO_ID);

            // Position (vec3)
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
            glVertexAttribDivisor(2, 1);

            // Color (vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
            glVertexAttribDivisor(3, 1);

            // Size (float)
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
            glVertexAttribDivisor(4, 1);

            glBindVertexArray(0);
        }
    }

    virtual void Update() override {
        float deltaTime = object->scene->deltaTime;

        // Update existing particles
        for (auto& particle : particles) {
            if (!particle.active) continue;

            particle.lifeRemaining -= deltaTime;
            if (particle.lifeRemaining <= 0.0f) {
                particle.active = false;
                continue;
            }

            // Physics integration
            particle.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime; // Gravity
            particle.position += particle.velocity * deltaTime;

            // Update visual properties
            float lifeRatio = particle.lifeRemaining / particle.lifetime;
            particle.color = glm::mix(colorEnd, colorStart, lifeRatio);
            particle.size = glm::mix(sizeEnd, sizeStart, lifeRatio);
        }

        // Spawn new particles
        int particlesToSpawn = spawnRate * deltaTime;
        for (int i = 0; i < particlesToSpawn; i++) {
            auto& particle = GetNextAvailableParticle();
            if (particle.active) break; // Pool full

            // Initialize particle
            particle.position = emitterPosition;
            particle.velocity = glm::vec3(0,1,0);// glm::linearRand(velocityRange[0], velocityRange[1]);
            particle.color = colorStart;
            particle.size = sizeStart;
            particle.lifetime = particle.lifeRemaining = 1.0f;
            particle.active = true;
        }

        // Update GPU buffer
        UpdateInstanceData();
    }

    void UpdateInstanceData() {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Particle), particles.data());
    }

    Particle& GetNextAvailableParticle() {
        for (auto& particle : particles)
            if (!particle.active) return particle;
        return particles[0]; // Fallback
    }
};