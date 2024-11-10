#pragma once

#include "aProjectileMovement.hpp"
#include "Action.h";
#include "sObject.h"
#include <glm/vec3.hpp> 
#include "cBasicFlyCamera/cBasicFlyCamera.h"
#include "Scene.hpp"
#include <glm/glm.hpp>


class aLinearProjectileMovement : public aProjectileMovement
{
public:
    void Start() override
    {
        // Initialize starting speed and direction towards target
    }

    void Update() override
    {
        glm::vec3 direction = glm::normalize(target - object->mesh->positionXYZ);
        speed += acceleration * object->scene->deltaTime;
        object->mesh->positionXYZ += direction * speed * object->scene->deltaTime;

        float threshold = 0.5f;
        if (glm::distance(object->mesh->positionXYZ, target) < threshold)
        {
            OnImpact();
        }

    }

protected:
    virtual void OnImpact()
    {
        // Trigger explosion or other impact effects
    }
};
