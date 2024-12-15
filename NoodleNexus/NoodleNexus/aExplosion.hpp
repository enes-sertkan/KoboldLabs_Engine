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
    Object* objectToStick = nullptr;
    glm::vec3 localPos;

    void Start() override
    {
        object->mesh->uniformScale = 1.f;
        object->mesh->objectColourRGBA = explosionColor;

        localPos = object->mesh->positionXYZ - objectToStick->mesh->positionXYZ;
    }

    void Update() override
    {

        if (object->mesh->uniformScale >= maxScale)
        {
            object->isActive = false;
            //     object->Destroy();
            return;
        }


        if (objectToStick)
        {
            object->mesh->positionXYZ = objectToStick->mesh->positionXYZ + localPos;
        }
        object->mesh->uniformScale += expansionRate * object->scene->deltaTime;

    }
};