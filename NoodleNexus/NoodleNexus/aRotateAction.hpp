#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "aRayCastPhysics2D.hpp"

class aRotateAction : public Action
{
public:
    float rotationSpeed = 1.0f;
    aRayCastPhysics2D physics;

    void Update() override
    {
        if (object && object->mesh) 
        {
           if ( physics.speed.z>0)
            
            object->mesh->rotationEulerXYZ.z += rotationSpeed * object->scene->deltaTime;
           else
            object->mesh->rotationEulerXYZ.z -= rotationSpeed * object->scene->deltaTime;
        }
    }
};
