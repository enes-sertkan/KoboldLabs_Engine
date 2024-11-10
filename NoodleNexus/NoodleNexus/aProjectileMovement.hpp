// Projectile.h
#pragma once
#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>

class aProjectileMovement : public Action
{
public:
    glm::vec3 target;
    float speed = 100.0f;
    float acceleration = 2.0f;

    void Start() override
    {
        // Initialize starting position and direction
    }

    void Update() override
    {
        glm::vec3 direction = glm::normalize(target - object->mesh->positionXYZ);
        speed += acceleration * object->scene->deltaTime;
        object->mesh->positionXYZ += direction * speed * object->scene->deltaTime;

        if (glm::distance(object->mesh->positionXYZ, target) < 0.5f) // Replace with suitable threshold
        {
            OnImpact();
        }
    }

protected:
    virtual void OnImpact()
    {
        // Add explosion or impact marker actions here
        object->scene->RemoveObject(object);
    }
};
