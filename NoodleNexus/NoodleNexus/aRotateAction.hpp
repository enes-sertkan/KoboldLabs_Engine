#pragma once

#include "Action.h"
#include "Scene.hpp"

class aRotateAction : public Action
{
public:
    float rotationSpeed = 1.0f;

    void Update() override
    {
        if (object && object->mesh) 
        {
            
            object->mesh->rotationEulerXYZ.z += rotationSpeed * object->scene->deltaTime;
        }
    }
};
