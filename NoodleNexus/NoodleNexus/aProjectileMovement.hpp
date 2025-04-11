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

    void UpdateRotation() {
        if (!object) return;
        if (!object->startTranform) return;
        if (!object->mesh) return;
        if (glm::length(speed) > 0.001f) {
            glm::vec3 direction = glm::normalize(speed);

            // Calculate yaw (horizontal rotation)
            float yaw = std::atan2(direction.x, direction.z);

            // Calculate pitch (vertical tilt)
            float pitch = std::asin(-direction.y);  // Correct vertical calculation

            // Create rotation matrix with YAW first, then PITCH
            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix = glm::rotate(rotationMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));  // Yaw
            rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch

            // Extract Euler angles in XYZ order from the combined matrix
            glm::vec3 euler;
            glm::extractEulerAngleXYZ(rotationMatrix, euler.x, euler.y, euler.z);

            // Convert to degrees and update rotation
            object->mesh->rotationEulerXYZ = glm::degrees(euler);
        }
    }

    void Update() override
    {
        ApplyAcceleration();
        ApplySpeed();
       
    }
};