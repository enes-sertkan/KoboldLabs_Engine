#pragma once
#include "sObject.h"

class Action
{
private:

public:
    double deltaTime;
    Object* object = nullptr;

    virtual void Start() {}

    // Update method with deltaTime parameter
    virtual void Update()
    {
      

        printf("WOW, you are UPDATING\n");

    }
};

class MoveForward : public Action
{
    void Update() override
    {
        // Use deltaTime to control movement speed
        object->mesh->positionXYZ.x += 1.0f; // Adjust this speed as needed
    }
};


class ExplosionLogic : public Action {
public:
    void Update() override {
        if (object->mesh->uniformScale >= 5) {
            return;
        }
        object->mesh->uniformScale += 0.5f * deltaTime;
    }
};

