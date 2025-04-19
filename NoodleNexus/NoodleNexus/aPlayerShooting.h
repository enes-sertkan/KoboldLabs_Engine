#pragma once

#include "Action.h"
#include "LabAttackFactory.h"   // Make sure this header is in your include path
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


class aPlayerShooting : public Action {
public:
    // Public pointer to the object factory (assign externally)
    LabAttackFactory* factory = nullptr;

    // Fire rate in seconds (time between shots)
    float fireRate = 0.1f;
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

        Object* slime = scene->GenerateMeshObjectsFromObject("assets/models/Sphere_radius_1_xyz_N_uv.ply", pos, 0.2, glm::vec3(0.f, 0.f, 0.f), true, glm::vec4(0.f, 1.f, 0.f, 1.f), true, scene->sceneObjects);
        SoftBody* slimeBody = new SoftBody();

        slime->isTemporary = true;
        slime->mesh->metal = 0.1f;
        slime->mesh->smoothness = 0.1f;
        slimeBody->acceleration.y = -16;
        slime->mesh->drawBothFaces = true;
        slimeBody->constIterations = 2;
        //slimeBody->sbCollision->collisionMult = 2.f;
        slimeBody->tighness = 2.f;
        //softObject->mesh->NMTexture = "Wall_Simple_Normal.bmp";
        // . . . . . . . . 
        slimeBody->useVolume = true;
        slimeBody->easyControl = true;
        // slimeBody->inCylynder = true;
        slimeBody->SetMazeToSBCollision(maze);
        slimeBody->initialVelocity = velocity;
        scene->AddActionToObj(slimeBody, slime);

        slimeBody->Start();
    }

};