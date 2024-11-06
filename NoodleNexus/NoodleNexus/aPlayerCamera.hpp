#pragma once
#include "Action.h"
#include "sObject.h" 
#include "Transform.h" 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class aPlayerCamera : public Action
{
private:
    glm::vec3 offset; 
    Transform* cameraTransform; 

public:
    // Constructor to initialize the camera transform and the offset
    aPlayerCamera(Transform* camTransform, glm::vec3 cameraOffset)
        : cameraTransform(camTransform), offset(cameraOffset)
    {
    }

    void Update() override
    {
        // Check if the player object and camera transform are valid
        if (object && object->startTranform && cameraTransform)
        {
            cameraTransform->position = object->startTranform->position + offset;

            glm::vec3 direction = object->startTranform->position - cameraTransform->position;

            if (glm::length(direction) > 0.0f)
            {
                direction = glm::normalize(direction);

                float yaw = glm::atan(direction.x, direction.z); 
                float pitch = glm::asin(direction.y); 

                glm::mat4 rotationMatrix = glm::mat4(1.0f);

                rotationMatrix = glm::rotate(rotationMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
                rotationMatrix = glm::rotate(rotationMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));

                //cameraTransform->rotation = rotationMatrix;
            }
        }
    }
};
