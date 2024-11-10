// Projectile.h
#pragma once
#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>

class aProjectileMovement : public Action
{
public:
    glm::vec3 target;
    glm::vec3 direction;
    float speed = 100.0f;
    float acceleration = 2.0f;

    void Start() override
    {
        // Initialize starting position and direction
        direction = glm::normalize(target - object->mesh->positionXYZ);

    }

    void Update() override
    {
        speed += acceleration * object->scene->deltaTime;
        object->mesh->positionXYZ += direction * speed * object->scene->deltaTime;


    }

protected:
    virtual void OnImpact()
    {
        // Add explosion or impact marker actions here
        object->scene->RemoveObject(object);

    }
};
