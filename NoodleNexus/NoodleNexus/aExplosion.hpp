// Explosion.h
#pragma once
#include "UsableItem.hpp"
#include <string>
#include <iostream>

class aExplosion : public Action
{
public:
    float expansionRate = 0.4f;
    float maxScale = 5.0f;
    glm::vec4 explosionColor = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);

    void Start() override
    {
        object->mesh->uniformScale = 0.1f;
        object->mesh->objectColourRGBA = explosionColor;
    }

    void Update() override
    {

        if (object->mesh->uniformScale >= maxScale)
        {
            object->isActive = false;
            return;
        }

        object->mesh->uniformScale += expansionRate * object->scene->deltaTime;

    }
};
