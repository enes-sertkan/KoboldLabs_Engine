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
        if (isOn && objectToFollow->name == "Hammer")
        {
            object->mesh->modelFileName = modelName;
            object->mesh->positionXYZ = objectToFollow->mesh->positionXYZ + offset;
            object->mesh->bIsVisible = true;
        }
        else
        {
            object->mesh->bIsVisible = false; // Consider only hiding the icon, not Mario
        }
    }

};