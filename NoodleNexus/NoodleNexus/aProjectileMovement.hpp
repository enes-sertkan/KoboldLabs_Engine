// Projectile.h
#pragma once
#include "UsableItem.hpp"
#include <vector>
#include <glm/glm.hpp>

class aProjectileMovement : public Action
{
public:
    glm::vec3 direction;
    float speed = 1.0f;
    float acceleration = 0.1f;

    void Start() override
    {
        // Initialize starting position and direction
        direction = glm::normalize(direction);

    }


    void ApplyAcceleration()
    {
        speed += acceleration * object->scene->deltaTime;
    }


    void ApplySpeed()
    {
        object->mesh->positionXYZ += direction * speed * object->scene->deltaTime;

    }

    void Update() override
    {
       

        ApplyAcceleration();
        ApplySpeed();


    }


};
