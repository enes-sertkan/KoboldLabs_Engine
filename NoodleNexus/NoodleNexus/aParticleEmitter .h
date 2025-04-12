#pragma once
#include "sObject.h"
#include <glm/glm.hpp>
#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib> 


class aParticleEmitter : public Action {
public:
    float randomFloat(float min, float max) {
        return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
    }

    glm::vec3 randomVelocity() {
        // Generate random components for each axis
        float x = randomFloat(minDirection.x, maxDirection.x);
        float y = randomFloat(minDirection.y, maxDirection.y);
        float z = randomFloat(minDirection.z, maxDirection.z);

        // Normalize to get direction
        glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));

        // Apply random speed
        float speed = randomFloat(velocityRange.x, velocityRange.y);
        return dir * speed;
    }

    std::vector<Particle>* particles = new   std::vector<Particle>();
    GLuint particleUBO;  // Changed to Shader Storage Buffer
    unsigned int maxParticles = 1024;
 
 
    // Helper function to generate a random 3D rotation speed:
    glm::vec3 randomRotationSpeed() {
        // Generate random components for each axis within the min/max range.
        float rx = randomFloat(minRotationSpeed.x, maxRotationSpeed.x);
        float ry = randomFloat(minRotationSpeed.y, maxRotationSpeed.y);
        float rz = randomFloat(minRotationSpeed.z, maxRotationSpeed.z);

        // Optionally, if you want directional randomness with a given magnitude,
        // normalize and multiply by a random speed factor (if needed):
        // glm::vec3 dir = glm::normalize(glm::vec3(rx, ry, rz));
        // float speed = randomFloat(someMin, someMax);
        // return dir * speed;

        return glm::vec3(rx, ry, rz);
    }


    // In aParticleEmitter, add rotation speed range parameters:
    glm::vec3 minRotationSpeed = glm::vec3(-200.0f, -200.0f, -200.0f);
    glm::vec3 maxRotationSpeed = glm::vec3(200.0f, 200.0f, 200.0f);

    // Particle properties
    glm::vec2 velocityRange = glm::vec2(0.1f, 1.f);
    glm::vec3 minDirection = glm::vec3(-0.1, 1, -0.1);
    glm::vec3 maxDirection = glm::vec3(0.1, 1, 0.1);

    glm::vec4 colorStart = glm::vec4(0.4, 0.4, 0.4, 0.8);
    glm::vec4  colorEnd = glm::vec4(0.6, 0.6, 0.6, 0);
    glm::vec3 damping = glm::vec3(1.f);
    float sizeStart = 0.1f;
    float sizeEnd = 1.f;
    float particlesToSpawn = 0;
    float spawnRate = 3.f;
    bool spawnActive = true;
    bool destroyOnNoParticles = true;
    glm::vec2 lifeTimeRange = glm::vec2(0.7, 1.5f);
    bool firstSpawn = true;
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
        
        if (!firstSpawn)
        {
            int counter = 0;

            // Update existing particles
            for (int i = 0; i < particles->size(); i++) {
                if (!particles->at(i).active) continue;
                particles->at(i).lifeRemaining -= deltaTime;
                if (particles->at(i).lifeRemaining <= 0.0f) {
                    particles->at(i).active = false;

                    continue;
                }

                counter++;
                // Physics integration
              //  particles->at(i).velocity += glm::vec3(0.0f, -9.81f, 0.0f) * deltaTime; // Gravity

    // Update rotation: add rotationSpeed (vec3) * deltaTime (in seconds)
                particles->at(i).rotation += particles->at(i).rotationSpeed * deltaTime;

                particles->at(i).position += particles->at(i).velocity * deltaTime;

                particles->at(i).velocity = particles->at(i).velocity * damping;
                particles->at(i).rotation = particles->at(i).rotation * damping;

                // Update visual properties
                float lifeRatio = particles->at(i).lifeRemaining / particles->at(i).lifetime;
                if (lifeRatio < 0) { particles->at(i).active = false; }
                particles->at(i).color = glm::mix(colorEnd, colorStart, lifeRatio);
                particles->at(i).size = glm::mix(sizeEnd, sizeStart, lifeRatio);



            }
            if (counter <= 0 && !spawnActive && destroyOnNoParticles)
            {
                this->object->Destroy();
            }
        }



        // Spawn new particles
         particlesToSpawn += spawnRate * deltaTime;
         if (spawnActive || firstSpawn)
        for (int i = 0; i < particlesToSpawn; i++) {
            int id = GetNextAvailableParticle();
      //      if (particles->at(id).active) break; // Pool full

            // Initialize particle
            for (int i = 0; i < particlesToSpawn; i++) {
                int id = GetNextAvailableParticle();
                // Initialize particle
                particles->at(id).position = object->GetWorldPosition();
                particles->at(id).velocity = randomVelocity();
                particles->at(id).color = colorStart;
                particles->at(id).size = sizeStart;
                particles->at(id).lifetime = particles->at(id).lifeRemaining = randomFloat(lifeTimeRange.x, lifeTimeRange.y);

                // NEW: Set initial rotation and random rotation speed
                particles->at(id).rotation = randomRotationSpeed();// or a random initial rotation if desired
                particles->at(id).rotationSpeed = randomRotationSpeed();

                particles->at(id).active = true;
            }
        }
        particlesToSpawn -= floorf(particlesToSpawn);



        firstSpawn = false;

        object->mesh->pParticles = particles;


     

    }


    void UpdateSSBOData() {
   
    
    }

    int GetNextAvailableParticle() {
        int i = 0;
        for (auto& particle : *particles)
        {
           
            if (!particle.active) return i;
            i++;
        }
      
          return 0;
    }

    void OnDestroy() override
    {
        for (Particle particle : *particles)
        {
            particle.active = false;
        }
        object->mesh->pParticles = particles;
        std::vector<GPUParticle> gpuParticles = GenerateGPUParticles(*particles);
        glBindBuffer(GL_UNIFORM_BUFFER, particleUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, gpuParticles.size() * sizeof(GPUParticle), gpuParticles.data());
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, particleUBO);

    }


    std::vector<GPUParticle>  GenerateGPUParticles(std::vector<Particle> cpuParticles) {

        std::vector<GPUParticle> gpuParticles;
        for (Particle cpuParticle : cpuParticles) {
     //       if (!cpuParticle.active) continue;




            gpuParticles.push_back(GPUParticle(cpuParticle));
        }

        return gpuParticles;

    }
};