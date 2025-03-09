#pragma once
#include "sObject.h"
#include "Scene.hpp"

class Object;

class aRotationWithMinutes : public Action
{
private:
    float currentRotation; // Current rotation angle
    float rotationSpeed;   // Speed of rotation
    bool rotatingRight;    // Direction of rotation

public:
    float minRotation = -0.5f; // Minimum rotation angle (degrees)
    float maxRotation = 0.5f;  // Maximum rotation angle (degrees)

    virtual void Start()
    {
        // Initialize rotation state
        currentRotation = 0.0f;
        rotationSpeed = 0.0f; // Degrees per second
        rotatingRight = true;  // Start rotating to the right
    }

    virtual void Update()
    {
        if (!object) return;

        // Calculate rotation delta based on time
        float rotationDelta = rotationSpeed * object->scene->deltaTime;

        // Update current rotation
        if (rotatingRight)
        {
            currentRotation += rotationDelta;
            if (currentRotation >= maxRotation)
            {
                currentRotation = maxRotation; // Clamp to max
                rotatingRight = false;          // Reverse direction
            }
        }
        else
        {
            currentRotation -= rotationDelta;
            if (currentRotation <= minRotation)
            {
                currentRotation = minRotation; // Clamp to min
                rotatingRight = true;           // Reverse direction
            }
        }

        // Apply rotation to the object
        //object->mesh->rotationEulerXYZ.y = currentRotation;
    }
};