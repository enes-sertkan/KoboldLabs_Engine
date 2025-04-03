// aTurretNeckRotate.hpp
#pragma once
#include "Action.h"
#include <glm/glm.hpp>
#include "sObject.h"
#include "Scene.hpp"
class aTurretNeckRotate : public Action 
{
public:
    float rotationSpeed = 45.0f; // Degrees per second

    void Update() override 
    {
        if (!object) return;
        
        // Rotate around Y axis
        float deltaRotation = rotationSpeed * object->scene->deltaTime;
        object->mesh->rotationEulerXYZ.y += deltaRotation;
    }

    Action* Clone() const override 
    {
        aTurretNeckRotate* clone = new aTurretNeckRotate(*this);
        clone->object = nullptr;
        return clone;
    }
};