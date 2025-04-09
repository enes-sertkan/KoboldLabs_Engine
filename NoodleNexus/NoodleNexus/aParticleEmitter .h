#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib> 


class aParticleEmitter : public Action {
private:
    float randomFloat(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    glm::vec3 randomVelocity() {
        // Generate random components for each axis
        float x = randomFloat(-1.0f, 1.0f);
        float y = randomFloat(-1.0f, 1.0f);
        float z = randomFloat(-1.0f, 1.0f);

        // Normalize to get direction
        glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));

        // Apply random speed
        float speed = randomFloat(velocityRange.x, velocityRange.y);
        return dir * speed;
    }

    std::vector<Particle>* particles = new   std::vector<Particle>();
    GLuint particleUBO;  // Changed to Shader Storage Buffer
    unsigned int maxParticles = 1024;
    float spawnRate = 3.f;

    // Particle properties
    glm::vec2 velocityRange = glm::vec2(5.f,10.f);
    glm::vec4 colorStart=glm::vec4(1);
    glm::vec4  colorEnd = glm::vec4(0.8,0.2,0.2,1);
    float sizeStart = 0.1f;
    float sizeEnd = 1.f;
    float particlesToSpawn = 0;
public:
    virtual void Start() override {
        particles->resize(maxParticles);

        // Create UBO instead of SSBO
        glGenBuffers(1, &particleUBO);
        glBindBuffer(GL_UNIFORM_BUFFER, particleUBO);
        glBufferData(GL_UNIFORM_BUFFER,
            maxParticles * sizeof(GPUParticle),
            nullptr,
            GL_DYNAMIC_DRAW);

        // Bind to index 0
        GLuint blockIndex = glGetUniformBlockIndex(object->scene->particleProgram, "ParticleData");
        glUniformBlockBinding(object->scene->particleProgram, blockIndex, 1);
        glBindBufferBase(GL_UNIFORM_BUFFER, 1, particleUBO);

        object->mesh->particleUBO = particleUBO;
        object->mesh->isParticleEmitter = true;
       // static_assert(sizeof(GPUParticle) == 64, "UBO size mismatch with shader!");
    }


    virtual void Update() override {
        float deltaTime = object->scene->deltaTime;

        // Update GPU buffer
      //  UpdateSSBOData();

        // Spawn new particles
         particlesToSpawn += spawnRate * deltaTime;
        for (int i = 0; i < particlesToSpawn; i++) {
            int id = GetNextAvailableParticle();
      //      if (particles->at(id).active) break; // Pool full

            // Initialize particle
            particles->at(id).position = object->GetWorldPosition();
            particles->at(id).velocity = randomVelocity();// glm::linearRand(velocityRange[0], velocityRange[1]);
            particles->at(id).color = colorStart;
            particles->at(id).size = sizeStart;
            particles->at(id).lifetime = particles->at(id).lifeRemaining = 10.f;
            particles->at(id).active = true;
        }
        particlesToSpawn -= floorf(particlesToSpawn);


        // Update existing particles
        for (int i = 0; i < particles->size(); i++) {
            if (!particles->at(i).active) continue;

            particles->at(i).lifeRemaining -= deltaTime;
            if (particles->at(i).lifeRemaining <= 0.0f) {
                particles->at(i).active = false;
                continue;
            }

            // Physics integration
          //  particles->at(i).velocity += glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime; // Gravity
            particles->at(i).position += particles->at(i).velocity * deltaTime;

            // Update visual properties
            float lifeRatio = particles->at(i).lifeRemaining / particles->at(i).lifetime;
            if (lifeRatio < 0) { particles->at(i).active = false; }
            particles->at(i).color = glm::mix(colorEnd, colorStart, lifeRatio);
            particles->at(i).size = glm::mix(sizeEnd, sizeStart, lifeRatio);

        }

        object->mesh->pParticles = particles;

    }


    void UpdateSSBOData() {
   
    
    }

    int GetNextAvailableParticle() {
        int i = 0;
        for (auto& particle : *object->mesh->pParticles)
        {
           
            if (!particle.active) return i;
            i++;
        }
      
          return 0;
    }
};