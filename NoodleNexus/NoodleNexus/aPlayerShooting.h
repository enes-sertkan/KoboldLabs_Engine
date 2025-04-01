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
        if (glfwGetMouseButton(object->scene->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
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
                glm::vec3 bulletSpeed = shootDir * 40.0f;  // Example: 50 units per second

                // Optionally, offset the spawn position in the shooting direction to avoid immediate collisions.
                glm::vec3 spawnPos = playerPos + shootDir * 1.0f;
                spawnPos.y -= 4.5f;
                // Spawn the player bullet using the factory.
                factory->SpawnPlayerBullet(spawnPos, bulletSpeed);

                // Reset the fire timer.
                timeSinceLastShot = 0.0f;
            }
        }
    }
};