#pragma once

#include "Action.h"
#include "LabAttackFactory.h"   // Make sure this header is in your include path
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "SlimeController.h"

class aPlayerShooting : public Action {
public:
    // Pool settings
     const int MAX_POOL_SIZE = 5;  // Change this value as needed
     std::vector<SoftBody*> slimePool;
    // Public pointer to the object factory (assign externally)
    LabAttackFactory* factory = nullptr;

    // Fire rate in seconds (time between shots)
    float fireRate = 0.8f;
    // Timer tracking time since last shot
    float timeSinceLastShot=0.f;

    
  

    virtual void Start() override
    {
        // Initialize the timer so the player can fire immediately on first press.
        timeSinceLastShot = fireRate;
    }
    virtual void Update() override
    {
        // Increase the timer based on delta time.
        timeSinceLastShot += object->scene->deltaTime;

        // Check for left mouse button press.
        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Only spawn a bullet if the fire rate delay has elapsed.
            if (timeSinceLastShot >= fireRate && factory) {
                // Get the player's current position.
                glm::vec3 playerPos = object->mesh->positionXYZ;
                playerPos.y += 4;
                // Get the camera's rotation (pitch, yaw) in degrees.
                glm::vec2 eyeRotation = object->scene->fCamera->getEyeRotation();
                // Convert yaw and pitch to radians.
                float yaw = glm::radians(eyeRotation.y);
                float pitch = glm::radians(eyeRotation.x);

                // Adjust yaw by subtracting 90 degrees so that when yaw is 0,
                // the shooting direction matches your movement (which is -left).
                float adjustedYaw = yaw - glm::radians(90.0f);

                // Compute the shooting direction using the adjusted yaw and pitch.
                // Using spherical coordinates:
                // shootDir.x = sin(adjustedYaw) * cos(pitch)
                // shootDir.y = sin(pitch)
                // shootDir.z = cos(adjustedYaw) * cos(pitch)
                glm::vec3 shootDir;
                shootDir.x = sin(adjustedYaw) * cos(pitch);
                shootDir.y = -sin(pitch);
                shootDir.z = cos(adjustedYaw) * cos(pitch);
                shootDir = glm::normalize(shootDir);

                // Define the bullet speed (adjust the magnitude as needed).
                glm::vec3 bulletSpeed = shootDir * 100.0f;  // Example: 50 units per second

                // Optionally, offset the spawn position in the shooting direction to avoid immediate collisions.
                glm::vec3 spawnPos = playerPos + shootDir * 1.0f;
                spawnPos.y -= 4.5f;
                // Spawn the player bullet using the factory.
              //  factory->SpawnPlayerBullet(spawnPos, bulletSpeed);
                SpawnSlime(object->scene, spawnPos, factory->maze, bulletSpeed);
                // Reset the fire timer.
                timeSinceLastShot = 0.0f;
            }
        }
    }



    void SpawnSlime(Scene* scene, glm::vec3 pos, MazeGenerator* maze, glm::vec3 velocity)
    {
        SoftBody* slimeBody = nullptr;
        Object* slimeObj = nullptr;

        // Generate random size between 0.15 and 0.5
        float randomScale = 0.15f + static_cast<float>(rand()) / RAND_MAX * (0.3f - 0.15f);

        // Generate random color (RGB between 0.3 and 1.0 for brightness)
        glm::vec4 randomColor = glm::vec4(
            0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f,
            0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f,
            0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f,
            1.0f
        );

        // Try to reuse oldest slime if pool is full
        if (slimePool.size() >= MAX_POOL_SIZE) {
            auto oldest = std::min_element(slimePool.begin(), slimePool.end(),
                [](const SoftBody* a, const SoftBody* b) {
                    return a->lastUsedTime < b->lastUsedTime;
                });

            slimeBody = *oldest;
            slimeObj = slimeBody->object;

            slimeBody->Reset();
            slimeBody->MoveTo(pos);
            slimeBody->initialVelocity = velocity;

            // Update appearance for reuse
            slimeObj->mesh->uniformScale = randomScale;
            slimeObj->mesh->objectColourRGBA = randomColor;
        }
        else {
            slimeObj = scene->GenerateMeshObjectsFromObject(
                "assets/models/Sphere_radius_1_xyz_N_uv.ply",
                pos, randomScale, glm::vec3(0.f),
                true, randomColor,
                true, scene->sceneObjects
            );

            slimeBody = new SoftBody();
            slimeBody->acceleration.y = -16;
            slimeBody->constIterations = 2;
            slimeBody->tighness = 2.f;
            slimeBody->useVolume = true;
            slimeBody->easyControl = true;
            slimeBody->SetMazeToSBCollision(maze);
            slimeBody->initialVelocity = velocity;

            scene->AddActionToObj(slimeBody, slimeObj);
            slimeBody->Start();

            SlimeController* slimeController = new SlimeController();
            slimeController->factory = factory;
            slimeController->softBody = slimeBody;

            scene->AddActionToObj(slimeController, slimeObj);

            if (factory->grass != nullptr)
            {
                aGrassCollider* grassCollider = new aGrassCollider();
                grassCollider->SetGrass(factory->grass);
                grassCollider->colliderRadius = 1.4f*randomScale/0.3f;
                grassCollider->colliderBlendRadius = 1.5f;
                grassCollider->softBody = slimeBody;
                scene->AddActionToObj(grassCollider, slimeObj);
            }

            slimePool.push_back(slimeBody);
        }

        slimeObj->mesh->positionXYZ = pos;
        slimeObj->isTemporary = true;
        slimeObj->mesh->metal = 0.8f;
        slimeObj->mesh->smoothness = 0.9f;
        slimeObj->mesh->drawBothFaces = true;

       

        slimeBody->lastUsedTime = glfwGetTime();
    }


};