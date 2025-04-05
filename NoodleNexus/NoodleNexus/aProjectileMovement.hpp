// Projectile.h
#pragma once
#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

class aProjectileMovement : public Action
{
public:
    glm::vec3 speed;

    void Start() override
    {
        // Initial rotation update if needed
        UpdateRotation();
    }

    void ApplyAcceleration()
    {
        //   speed += acceleration * object->scene->deltaTime;
    }

    void ApplySpeed()
    {
        object->mesh->positionXYZ += speed * object->scene->deltaTime;
    }

    void UpdateRotation()
    {
        if (glm::length(speed) > 0.001f)
        {
            // Calculate direction from speed vector
            glm::vec3 direction = glm::normalize(speed);

            // Calculate yaw (horizontal rotation)
            float yaw = std::atan2(direction.x, direction.z);

            // Calculate pitch (vertical tilt)
            float horizontalSpeed = glm::length(glm::vec3(direction.x, 0.0f, direction.z));
            float pitch = 0.0f;

            if (horizontalSpeed > 0.001f) {
                pitch = std::atan2(-direction.y, horizontalSpeed);
            }
            else {
                // Handle pure vertical movement
                pitch = (direction.y > 0) ? -glm::half_pi<float>() : glm::half_pi<float>();
            }

            // Convert to degrees and update rotation
            object->mesh->rotationEulerXYZ = glm::vec3(
                glm::degrees(pitch),
                glm::degrees(yaw),
                0.0f  // No roll by default
            );
        }
    }

    void Update() override
    {
        ApplyAcceleration();
        ApplySpeed();
       
    }
};