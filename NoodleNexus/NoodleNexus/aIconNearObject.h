#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aRayCastPhysics2D.hpp"
#include "aModelsFramesAnimator.hpp"
#include "aRayCastPhysics2D.hpp"

class aIconNearObject : public Action
{
public:

    std::string modelName;
    bool isOn = false;
    Object* objectToFollow = new Object();
    glm::vec3 offset;


    void Update() override
    {
        if (isOn)
        {
            object->mesh->modelFileName = modelName;
            object->mesh->positionXYZ = objectToFollow->mesh->positionXYZ + offset;
            object->mesh->bIsVisible = true;

            // Get the player's current rotation (facing direction).
            float playerRotationY = objectToFollow->mesh->rotationEulerXYZ.y;

            // Flip hammer's rotation based on the player's direction.
            if (playerRotationY > 90)
            {
                object->mesh->rotationEulerXYZ.x = -45.0f;  // Facing left.
                offset = glm::vec3(0.0f, 9.0f, -4.0f);
            }
            else
            {
                object->mesh->rotationEulerXYZ.x = +45.0f;    // Facing right.
                offset = glm::vec3(0.0f, 9.0f, 4.0f);
            }
        }
        else
        {
            object->mesh->bIsVisible = false;
        }
    }


};