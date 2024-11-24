#pragma once

#include "Action.h"
#include "Scene.hpp"
#include "sObject.h"

class aRemoveAfterTime : public Action
{
public:
    float timeToRemove = 5.0f; // Time after which the object will be removed
    float elapsedTime = 0.0f;   // Tracks elapsed time


    void Update() override
    {
        elapsedTime += object->scene->deltaTime;

        // Check if the time has elapsed
        if (elapsedTime >= timeToRemove)
        {
            // Remove object from the scene
            object->Destroy();
        }
    }
};
